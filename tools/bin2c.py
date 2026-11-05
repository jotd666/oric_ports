
import os,sys,shutil,re,csv,bisect
import getopt,traceback,python_lacks


EQS = """
EQUI PARAM1 = $02E1
EQUI PARAM2 = $02E3
EQUI PARAM3 = $02E5
EQUI VIA_PB = $0300
EQUI VIA_PA       = $0301
EQUI VIA_DDRB      = $0302
EQUI VIA_DDRA      = $0303
EQUI VIA_T1CL      = $0304
EQUI VIA_T1CH      = $0305
EQUI VIA_T1LL      = $0306
EQUI VIA_T1LH      = $0307
EQUI VIA_T2CL      = $0308
EQUI VIA_T2CH      = $0309
EQUI VIA_SR       = $030A
EQUI VIA_ACR       = $030B
EQUI VIA_PCR      = $030C
EQUI VIA_IER      = $030E
EQUI VIA_PANH      = $030F
EQUI TIMER2      = $0310
EQUI TEMP1       = $0312
EQUI TEMP2       = $0314
EQUI TEMP3       = $0315
EQUI SAVIO       = $0316
EQUI TIMFLG      = $0317
EQUI STACKP      = $0318
EQUI TSTAT       = $0319
EQUI HATABS      = $031A
EQUI PUPBT1      = $033D
EQUI PUPBT2      = $033E
EQUI PUPBT3      = $033F
EQUI IOCB0       = $0340
EQUI IOCB1       = $0350
EQUI IOCB2       = $0360
EQUI IOCB3       = $0370
EQUI IOCB4       = $0380
EQUI IOCB5       = $0390
EQUI IOCB6       = $03A0
EQUI IOCB7       = $03B0
EQUI PRNBUF      = $03C0
EQUI SUPERF      = $03E8
EQUI CKEY        = $03E9
EQUI CASSBT      = $03EA
EQUI CARTCK      = $03EB
EQUI DERRF       = $03EC
EQUI ACMVAR      = $03ED
EQUI BASICF      = $03F8
EQUI MINTLK      = $03F9
EQUI GINTLK      = $03FA
EQUI CHLINK      = $03FB
EQUI CASBUF      = $03FD

EQUI ORIC1_READKEY = $C5E8
EQUI ORICA_CLS = $CCCE
EQUI ORICA_READ_BYTE_FROM_TAPE = $E6C9
EQUI ORICA_CURSET = $F0C8
EQUI ORIC1_W8912 = $F535
EQUI ORICA_DRAW = $F110
EQUI ORICA_CIRCLE = $F37F
EQUI ORICA_FILL = $F268
EQUI ORICA_PAPER = $F204
EQUI ORICA_INK = $F210
EQUI ORICA_PLAY  = $FBD0
EQUI ORICA_SOUND = $FB40
EQUI ORICA_W8912 = $F590
EQUI ORICA_RESET_CHARSET = $F8D0
EQUI ORICA_SET_6522_FOR_CASSETTE = $E76A
EQUI ORICA_RESET_CASSETTE_STATUS = $E93D
EQUI ORICA_HIRES = $EC33
EQUI ORICA_LORES = $D9DE
EQUI ORICA_PRINT_INTEGER_IN_AX = $E0C5
EQUI ORICA_CIRCLE_CURMOV = $EAF0
EQUI ORICA_PRINT_STRING_AFTER_QUOTE = $CCB0
EQUI ORICA_TEXT = $EC21
EQUI ORICA_GET_IN_SYNC_WITH_CASSETTE = $E735
EQUI ORICA_CHECK_FOR_KEY = $EB78
EQUI ORICA_PRINT_CHARACTER = $CCD9
EQUI ORICA_OUTPUT_BYTE_TO_CASSETTE = $E65E
EQUI ORICA_NEWLINE = $CBF0
EQUI ORICA_HARD_RESET = $F88F

EQUI ORIC1_PRINT_INTEGER_IN_AX = $E0C1
EQUI ORIC1_TEXT = $E9A9
EQUI ORIC1_CLS = $CC0A
EQUI ORIC1_HIRES = $E9BB
EQUI ORIC1_CHECK_FOR_KEY = $E905
EQUI ORIC1_PRINT_STRING_AFTER_QUOTE = $CBED
EQUI ORIC1_NEWLINE = $CB9F
EQUI ORIC1_HARD_RESET = $F84A
EQUI ORIC1_SET_6522_FOR_CASSETTE = $E6CA
EQUI ORIC1_OUTPUT_BYTE_TO_CASSETTE = $E5C6
EQUI ORIC1_READ_BYTE_FROM_TAPE = $E630
EQUI ORIC1_GET_IN_SYNC_WITH_CASSETTE = $E696
EQUI ORIC1_PRINT_CHARACTER = $CC12

"""

equ2addr = {}
for line in EQS.splitlines():
    toks = line.split()
    if len(toks)==4 and toks[2]=="=":
        equ2addr[toks[1]] = int(toks[3][1:],16)


if str is not bytes:
    ord = lambda x:x


class Bin2C:
    __VERSION_NUMBER = "1.1"
    __LD_RE = re.compile("A[695D]")
    __CMP_RE = re.compile("C[9]")
    __J_RE = re.compile("4C")
    __OTHER_RE = re.compile("EA|60|18")
    try:
        __MODULE_FILE = __file__
    except:
        __MODULE_FILE = sys.executable
    __PROGRAM_NAME = os.path.basename(__MODULE_FILE)
    __PROGRAM_DIR = os.path.abspath(os.path.dirname(__MODULE_FILE))
    __COMMANDS = ["bin2asm","asm2c","all"]



    def __init__(self):
        self.__logfile = ""
        self.__temp_directory = None
        # init members with default values
        self.__input_file = None
        self.__directives = None
        self.__output_directory = None
        self.__command = None
        self.__multipass = True
        self.__used_symbols = dict()
        self.__start_offset = 0

        try:
            # set binary mode on output/error stream for Windows
            import msvcrt
            msvcrt.setmode (sys.stdout.fileno(), os.O_BINARY)
            msvcrt.setmode (sys.stderr.fileno(), os.O_BINARY)
        except:
            pass
    def __create_temp_directory(self):
        """
        defines self.__temp_directory
        """

        self.__temp_directory = os.path.join(os.getenv("TEMP"),self.__PROGRAM_NAME.replace(".py","")+("_%d" % os.getpid()))
        python_lacks.create_directory(self.__temp_directory)

    def __delete_temp_directory(self):
        if self.__temp_directory != None:
            [rc,output] = python_lacks.rmtree(self.__temp_directory)
            if rc != 0:
                self.__warn("Could not delete temp dir %s: %s" % (self.__temp_directory,output))

    def _init_from_sys_args(self,debug_mode = True):
        """ standalone mode """

        try:
            self.__do_init()
            self.__delete_temp_directory()
        except Exception:
            self.__delete_temp_directory()
            if debug_mode:
                # get full exception traceback
                traceback.print_exc()
            else:
                self.__message(python_lacks.ascii_compliant(sys.exc_info()[1].message, best_ascii_approximation=True))

                sys.exit(1)

# uncomment if module mode is required
##    def init(self,output_file):
##        """ module mode """
##        # set the object parameters using passed arguments
##        self.__output_file = output_file
##        self.__purge_log()
##        self.__doit()

    def __do_init(self):
        #count_usage.count_usage(self.__PROGRAM_NAME,1)
        self.__parse_args()
        self.__purge_log()
        self.__doit()

    def __purge_log(self):
        if self.__logfile:
            try:
                os.remove(self.__logfile)
            except:
                pass

    def __message(self,msg,with_prefix=True):
        if with_prefix:
            msg = self.__PROGRAM_NAME+(": %s" % msg)+os.linesep
        else:
            msg += "\n"
        try:
            sys.stderr.write(msg)
            sys.stderr.flush()
        except:
            # ssh tunneling bug workaround
            pass
        if self.__logfile:
            f = open(self.__logfile,"a")
            f.write(msg)
            f.close()

    def __error(self,msg,user_error=True):
        """
        set user_error to False to trigger error report by e-mail
        """
        raise Exception("*** Error: "+msg+" ***")

    def __warn(self,msg):
        self.__message("*** Warning: "+msg+" ***")

    class CodeLine:
        def __init__(self,address,text,used_symbols):
            self.text = text
            self.goto = False
            self.gosub = False
            self.ret = False
            self.address = address
            self.size = 1

            self.opcode = None
            self.param = None
            self.param_value = None

            self.goto_dest = None
            self.gosub_dest = None

            self.__used_symbols = used_symbols

        def pass1(self):
            """ analyse the line, and note down goto/gosub destinations
            (do not mark other lines with goto/gosub since they may not have been
            built yet)
            """
            args = self.text.split()
            if not args:
                print("no fields: %s"%self.text)
            else:
                self.opcode = args[0]
                self.param = None
                if len(args)>1:
                    self.size = 2
                    indirection = "(" in args[1]
                    if indirection:
                        args[1]=args[1].replace("(","").replace(")","")
                    opargs = args[1].split(",")
                    main_arg = opargs[0]

                    if indirection:
                        # indirection
                        self.opcode += "_ind"
                        self.param = main_arg
                    elif main_arg[0]=="#":
                        self.opcode += "_imm"
                        self.param = main_arg[1:]
                    elif main_arg=="A":
                        self.opcode += "_A"
                        self.size = 1
                    else:
                        # direct: size can be 3, we can only guess (unless we look at the memory too...)
                        self.param = main_arg
                    if len(opargs)==2:
                        # indexed
                        self.opcode += "_"+opargs[1]

                if self.param:
                    if self.param[0]=="$":
                        self.param = "0x"+self.param[1:]
                        self.param_value = int(self.param,16)
                        if self.param_value > 0xff:
                            self.size = 3
                    elif self.param[1]=="_":
                        self.param = "0x"+self.param[2:]
                        self.param_value = int(self.param,16)
                        if self.param_value > 0xff:
                            self.size = 3
                    else:
                        # try to get value from param name
                        self.param_value = equ2addr.get(self.param)
                        if self.param_value:
                            # store to generate a #define in the C part
                            self.__used_symbols[self.param_value] = self.param

                if self.opcode in ["rts","rte"]:
                    self.ret = True
                elif self.opcode=="brk":
                    self.size = 1
                elif self.opcode=="jmp" or (self.opcode[0]=="b" and self.opcode != "bit"):
                    # branch opcode: get destination
                    if self.opcode=="jmp":
                        self.size = 3
                    else:
                        self.size = 2
                    self.goto_dest = self.param_value
                elif self.opcode=="jsr":
                    # branch sub opcode: get destination
                    self.size = 3
                    self.gosub_dest = self.param_value
                elif self.opcode==".by":
                    self.opcode="illegal"
                    self.size = 1
                # TODO: indirect jsr/jmp

        def pass2(self,lines,address_list):
            """
            now all lines have been parsed, mark other lines as goto/gosub destinations
            """
            if self.gosub_dest and self.gosub_dest < 0xC000:
                if self.gosub_dest in lines:
                    lines[self.gosub_dest].gosub = True
                else:
                    print("Warning: call address %04X not in program" % self.gosub_dest)
            if self.goto_dest and self.goto_dest < 0xC000:
                if self.goto_dest in lines:
                    lines[self.goto_dest].gosub = True
                else:
                    print("Warning: branch address %04X not in program" % self.goto_dest)

            # check for probable self-modifying code
            if self.opcode.startswith("st"):
                # store instruction
                store_target = self.param_value
                try:
                    idx = bisect.bisect_left(address_list,store_target)

                    if 0<idx<len(address_list):
                        current_instruction_address = address_list[idx-1]
                        if store_target-current_instruction_address < lines[current_instruction_address].size:
                            print("Warning: probable self-modifying code at %04X (write to %04X)" % (self.address,store_target))

                except TypeError:
                    print("warning: wierd STA parameter",self.text)
        def to_c(self,merged_dirlines):
            # once parsing has been done for all lines, generate code
            rval=[]
            if self.gosub or self.goto or self.address in merged_dirlines:
                rval.extend(["label_0x%04X:" % self.address,"\n"])
            rval.append("/* %04X */ " % self.address)
            rval.append(self.opcode)
            if self.param:
                rval.extend(["(",self.param,")"])
            rval.extend([";","\n"])
            return rval

    def __parse_args(self):
        # Command definition
        # Prepare short & long args from list to avoid duplicate info

        # Help's text fields
        self.__opt_string = ""
        self.__opthelp = ""

        parameters = [["version","shows version"],
        ["help","shows this help"],
        ["input=","specify binary input file"],
        ["entrypoint=","specify input entrypoints file (hex offset values, 1 per line)"],
        ["output=","specify output directory"],
        ["command=","specify command among %s" % ",".join(self.__COMMANDS)],
        ["No-multipass","just disassemble entrypoints, do not try to find more code"],
        ["start-offset=","start offset (default 0)"],
        ]

        # Possible options for user
        options = []

        for oc in parameters:
            # Filling empty shortopts
            if len(oc) == 2:
                # oc[0] is currently the longopt
                # oc[0][0] will be his first letter
                oc.insert(0, oc[0][0])

            # Those variables will mainly be used for display
            short_opt = oc[0]
            long_opt = oc[1]
            desc = oc[2]

            has_args = long_opt.endswith("=")
            if has_args:
                # oc[0] is currently the short opt
                oc[0] += ":"
                long_opt = long_opt[:-1]

            ### HELP TEXT INIT [START] ###
            self.__opt_string += " -"+short_opt+"|--"+long_opt
            if has_args:
                self.__opt_string +=" <>"

            self.__opthelp += "-"+short_opt+" / --"+long_opt+" : "+desc+os.linesep
            ### HELP TEXT INIT [END] ###

            options.append("--"+long_opt)

        # Get all short ops
        short_opts = "".join([param[0] for param in parameters])
        # Get all long opts
        long_opts = [param[1] for param in parameters]
        # Get used opts and args
        used_opts, used_args = getopt.getopt(sys.argv[1:], short_opts, long_opts)

        # Command options
        for option, value in used_opts:
            if value.startswith("-"):
                self.__error("Parameter of %s is also an option (%s). Value omitted" % (option,value))
            oi = 0
            if option in ('-v',options[oi]):
                print(self.__PROGRAM_NAME + " v" + self.__VERSION_NUMBER)
                sys.exit(0)
            oi += 1
            if option in ('-h',options[oi]):
                self.__usage()
                sys.exit(0)
            oi += 1
            if option in ('-i',options[oi]):
                self.__input_file = os.path.abspath(value)
            oi += 1
            if option in ('-e',options[oi]):
                self.__directives = os.path.abspath(value)
            oi += 1
            if option in ('-o',options[oi]):
                self.__output_directory = value
            oi += 1
            if option in ('-c',options[oi]):
                self.__command = value
            oi += 1
            if option in ('-N',options[oi]):
                self.__multipass = False
            oi += 1
            if option in ('-s',options[oi]):
                self.__start_offset = int(value,16)

        if self.__input_file == None:
            self.__error("input file not set")
        if self.__output_directory == None:
            self.__error("output dir not set")
        if self.__command == None:
            self.__error("command not set")

    def __usage(self):
        sys.stderr.write("Usage: "+self.__PROGRAM_NAME+self.__opt_string+os.linesep)
        sys.stderr.write(self.__opthelp)

    def __dump_line(self,linebuf,f,i,end_offset):
            for j,d in enumerate(linebuf):
                f.write("0x%02x" % d)
                if j+i-7<end_offset-1:
                    f.write(", ")
                else:
                    f.write("  ")
            f.write('    // %04X: "' % (i-7))
            for d in linebuf:
                if d<32 or d>=127:
                    f.write(".")
                else:
                    f.write(chr(d))
            f.write('"\n')

    def __doit(self):
        # main processing here
        # parse directive file
        directives=[]

        if self.__command in self.__COMMANDS:
            pass
        else:
            self.__usage()
            self.__error("Unknown command '%s', valid commands are: %s" % (self.__command,",".join(self.__COMMANDS)))

        bin2asm = False
        asm2c = False
        if self.__command == "bin2asm":
            bin2asm = True
            python_lacks.create_directory(self.__output_directory)
        elif self.__command == "asm2c":
            asm2c = True
            if not os.path.isdir(self.__output_directory):
                self.__error("output/work directory %s does not exist, run other commands first" % self.__output_directory)
        elif self.__command == "all":
            bin2asm = True
            asm2c = True
            python_lacks.create_directory(self.__output_directory)

        if self.__directives:
            directives = python_lacks.read_file(self.__directives,remove_blank_lines=True)

        asm_lines = None

        # for all commands, we must read the binary input file
        f = open(self.__input_file,"rb")
        offset = 0
        contents = f.read()
        f.close()

        if asm2c:
            # generate C file containing Oric memory
            h_data = os.path.join(self.__output_directory,"data.h")
            f = open(h_data,"w")
            f.write("static unsigned char block[] = {\n")
            linebuf = []

            end_offset = len(contents)

            for i,c in enumerate(contents):
                linebuf.append(ord(c))
                if len(linebuf)==8:
                    self.__dump_line(linebuf,f,i,end_offset)
                    linebuf = []
            if linebuf:
                self.__dump_line(linebuf,f,(end_offset//8) * 8,end_offset)
            f.write("};\n")
            f.close()

        input_file_copy = os.path.join(self.__output_directory,"input.bin")
        output_file = os.path.splitext(input_file_copy)[0]+".A65"

        user_dirlines = set()
        if bin2asm:
            if not directives:
                self.__error("No entrypoint file/no directives")

            detected_code = dict()
            shutil.copyfile(self.__input_file,input_file_copy)
            start_address = 0
            for d in directives:
                d = d.strip().replace("$","").replace("0x","")
                if d[0]!="#": # comment
                    user_dirlines.add(int(d,16))
                    if start_address==0:
                            start_address = int(d,16)

            pass_number = 0

            merged_dirlines = set(user_dirlines)

            user_dirlines = sorted(user_dirlines)

            while True:
                if self.__multipass:
                    pass_number += 1
                    self.__message("Pass %d" % pass_number)


                dk = sorted(detected_code)
                for d in dk:
                    merged_dirlines.add(d)

                dirfile = os.path.join(self.__output_directory,"input.DIR")
                f = open(dirfile,"w")
                f.write("ROM input.bin\nLOAD ${0:04x}\nSTART ${0:04x}\n".format(self.__start_offset))
                f.write(EQS)
                for d in sorted(merged_dirlines):
                    f.write("EXAM $%04X\n" % d)
                f.close()

                os.environ["PATH"] = self.__PROGRAM_DIR + os.pathsep + os.environ["PATH"]

                # calling AUTO-DIS by Ruud Baltissen
                import subprocess
                # 16 bit cmd line
                #cmdline = ("ad","/Finput.dir","/B$%04X" % offset,"/S$%04X" % start_address,"/Dinput.DIR")
                cmdline = ("ad","input.dir")
                self.__message("Running ADIS...")
                p = subprocess.Popen(cmdline,cwd=self.__output_directory,stdin=subprocess.PIPE)
                rc,out = p.communicate(b"\n")
                if rc:
                    self.__message("ADIS failed")
                else:
                    self.__message("ADIS ended successfully")

                # post-process ASM file
                # detect "forgotten" code zones
                asm_lines = python_lacks.read_file(output_file)

                detected_code = dict()
                for i,a in enumerate(asm_lines):
                    if a.startswith("S_"): # data section
                        # grab the first data line
                        points = 0
                        for j in range(i+1,len(asm_lines)):
                            l = asm_lines[j]
                            if l.startswith(".by"):
                                secstart = int(a[2:].rstrip().rstrip(":"),16)
                                fields = python_lacks.awkfields(l.replace(","," "))
                                first_instruction = fields[1][1:]
                                if self.__LD_RE.match(first_instruction):
                                    points += 2
                                    if first_instruction[1] in ["D"]:
                                        # 16 bit
                                        next_instruction = fields[4][1:]
                                    else:
                                        next_instruction = fields[3][1:]
                                    if next_instruction[0] in ["8","9"]:
                                        # STORE instruction
                                        points += 2
                                    elif self.__LD_RE.match(next_instruction):
                                        points += 1

                                elif self.__CMP_RE.match(first_instruction):
                                    points += 1
                                elif self.__J_RE.match(first_instruction):
                                    points += 2
                                elif self.__OTHER_RE.match(first_instruction):
                                    points += 1
                                if points>0:
                                    self.__message("S_%04X (code likeliness: %d): %s, " % (secstart,points,",".join(fields[1:])))
                                    # check if section start is not too close from a user entrypoint
                                    insert_it = True
                                    # too close from an user entrypoint: don't insert it
                                    for i,a in enumerate(user_dirlines):
                                        delta=a-secstart
                                        if abs(delta)<8:
                                            insert_it = False
                                            break
                                    if insert_it:
                                        detected_code[secstart] = points
                                break
                if not self.__multipass:
                    break
                if not detected_code:
                    self.__message("No more code sections detected. Remaining data sections:")
                    for i,a in enumerate(asm_lines):
                        if a.startswith("S_"): # data section
                            # grab the first data line
                            points = 0
                            for j in range(i+1,len(asm_lines)):
                                l = asm_lines[j]
                                if l.startswith(".by"):
                                    secstart = a[2:]
                                    self.__message("S_%s: %s" % (secstart,l))
                                    break
                    break
                else:
                    self.__message("Press any key to continue")
                    input()

                # convert to macroized C
        if asm2c:
            if asm_lines==None:
                asm_lines = python_lacks.read_file(output_file)
            self.lines = dict()
            address = 0
            # pass 1: base parsing & create CodeLine objects, compute addresses
            for line in asm_lines:
                code_line = False
                entrypoint = False
                # remove (automatic) comments & leading blanks
                line = line.split(";")[0].rstrip()
                if line == "":
                    pass
                elif line.startswith(".by"):
                    # just increase current address
                    address += line.count(",")+1
                elif line.startswith("."):
                    # ignore
                    pass

                elif line[0] == "S": # data section
                    pass
                elif line[1] in ["_"]:
                    # compute address & resync if necessary
                    declared_address = int(line[2:6],16)
                    if declared_address != address:
                        self.__warn("Inaccurate address calculation: %04X, found %04X" % (address,declared_address))
                        address = declared_address
                    entrypoint = line[0]=="E"
                    # remove label info
                    code_line = True
                    line = line[7:].strip()
                else:
                    code_line = True
                    line = line.strip()
                if code_line:
                    if line:
                        cl = self.CodeLine(address,line,self.__used_symbols)
                        cl.gosub = entrypoint
                        cl.pass1()
                        self.lines[address] = cl
                        address += cl.size

            self.address_list = sorted(self.lines)

            # pass 2: scan through the objects (in any order)
            # and mark other objects if needed for goto/gosub
            for cl in self.lines.values():
                cl.pass2(self.lines,self.address_list)


            # now we can generate the code
            rval = ['#include "oric_machine.h"\n','#include "data.h"\n','#include "opcodes.h"\n','#include "oric_basic.h"\n','#include "constants.h"\n\n']

            call_handler_proto = ["void call_handler(int address,int caller_address,int from_basic)"]

            rval.extend(call_handler_proto)

            rval.extend([";","\n","\n","void mem_init()\n","{","\n","memcpy","(","cpu_get_mem","(",")",",","block",",","sizeof","(","block",")",")",";","\n","}","\n"])
            rval.extend(["void asm_entrypoint()\n","{","\n","mem_init","(",")",";","\n"])

            # entrypoint just calls the call_handler on the first line
            self.lines[self.address_list[0]].gosub = True

            rval.extend(["call_handler","(","0x%04X" % self.address_list[0],",","-1",",","1",")",";","\n","}","\n","\n"])
            rval.extend(call_handler_proto)
            rval.extend(["\n","{","\n","switch(address)","\n","{"])
            print(merged_dirlines)
            for address in self.address_list:
                if self.lines[address].gosub or address in merged_dirlines:
                    rval.extend(["\n","case"," ","0x%04X" % address,":"," ","goto"," ","label_0x%04X" % address,";"])

            rval.extend("""
        default:
    if (address>0xC000)
	  {
	    // try rom call
	    rom_call(address);


	  }
	else
	  {
	    error("CALL: unsupported address ",hex$(address)," called from ",from_basic ? "basic" : "asm"," line ",str$(caller_address),NULL);
        // this is never reached
        }
    }
     // and return to whatever (else it jumps to the next label!)
    rts;

""")

            opcode_dict = dict()

            previous_is_ret = False
            for a in self.address_list:
                cl = self.lines[a]
                if cl.opcode=="illegal":
                    self.__warn("$%04X: illegal instruction $%02X" % (cl.address,cl.param_value))
                elif cl.opcode in ["jsr","jmp"] and cl.param_value>=0xC000:
                    self.__warn("$%04X: ROM call to $%04X (%s)" % (cl.address,cl.param_value,cl.param))
                if previous_is_ret and not (cl.gosub or cl.goto):
                    self.__warn("$%04X: (statically) unreacheable code, adding label" % (cl.address))
                    #cl.gosub = True
                rval.extend(cl.to_c(merged_dirlines))
                if not cl.opcode in opcode_dict:
                    opcode_dict[cl.opcode] = 0
                opcode_dict[cl.opcode] += 1

                previous_is_ret = cl.ret

            rval.extend(["}","\n"])

            # write stats file
            output_stats_file = os.path.join(self.__output_directory,"stats.csv")
            osh = open(output_stats_file,"w",newline="")
            cw = csv.writer(osh,delimiter=";",quotechar='"')
            cw.writerow(["instruction","count"])
            oks = sorted(opcode_dict.keys())


            for i in oks:
                cw.writerow([i,opcode_dict[i]])

            osh.close()
            # write file
            output_c_file = os.path.join(self.__output_directory,"asm.c")
            output_handle = open(output_c_file,"w")
            output_handle.write("/* generated by JOTD's %s version %s */\n\n" % (self.__PROGRAM_NAME,self.__VERSION_NUMBER))
            if self.__used_symbols:
                for k,v in sorted(self.__used_symbols.items()):
                    output_handle.write("#define {} 0x{:X}\n".format(v,k))
                output_handle.write("\n")

            level = 0
            previous_t = None
            for t in rval:
                ts = t.strip()
                if ts!='}' and previous_t and previous_t[-1]=="\n":
                    output_handle.write("   "*level)
                if ts=='{':
                    level += 1
                elif ts=='}':
                    level -= 1
                    output_handle.write("   "*level)
                previous_t = t
                output_handle.write(t)
            output_handle.close()


if __name__ == '__main__':
    """
        Description :
            Main application body
    """


    o = Bin2C()
    o._init_from_sys_args(debug_mode = True)
    #o.init("output_file")
