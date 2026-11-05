

import os,sys,re
import getopt,traceback,python_lacks


class Tap2Bas:
    __VERSION_NUMBER = "1.0"
    try:
        __MODULE_FILE = sys.modules[__name__].__file__
    except:
        __MODULE_FILE = sys.executable
    __PROGRAM_NAME = os.path.basename(__MODULE_FILE)
    __PROGRAM_DIR = os.path.abspath(os.path.dirname(__MODULE_FILE))

    def __init__(self):
        self.__logfile = ""
        self.__temp_directory = None
        # init members with default values
        self.__output_file = None
        self.__input_file = None
        self.__fix_source = False
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
        finally:
            self.__delete_temp_directory()


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
        if self.__logfile != "":
            try:
                os.remove(self.__logfile)
            except:
                pass

    def __message(self,msg,with_prefix=True):
        if with_prefix:
            msg = self.__PROGRAM_NAME+(": %s" % msg)+os.linesep
        else:
            msg += os.linesep
        try:
            sys.stderr.write(msg)
            sys.stderr.flush()
        except:
            # ssh tunneling bug workaround
            pass
        if self.__logfile != "":
            f = open(self.__logfile,"ab")
            f.write(msg)
            f.close()

    def __error(self,msg,user_error=True):
        """
        set user_error to False to trigger error report by e-mail
        """
        raise Exception("*** Error: "+msg+" ***")

    def __warn(self,msg):
        self.__message("*** Warning: "+msg+" ***")

    def __parse_args(self):
        # Command definition
        # Prepare short & long args from list to avoid duplicate info

        # Help's text fields
        self.__opt_string = ""
        self.__opthelp = ""

        parameters = [["version","shows version"],
        ["help","shows this help"],
        ["input=","specify input TAP file"],
        ["output=","specify output BAS file"],
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
                self.__input_file = value
            oi += 1
            if option in ('-o',options[oi]):
                self.__output_file = value

        if self.__output_file == None:
            self.__error("output file not set")
        if self.__input_file == None:
            self.__error("input file not set")

    def __usage(self):
        sys.stderr.write("Usage: "+self.__PROGRAM_NAME+self.__opt_string+os.linesep)
        sys.stderr.write(self.__opthelp)

    __TOKEN = dict()
    __TOKEN[216] = "abs"
    __TOKEN[209] = "and"
    __TOKEN[236] = "asc"
    __TOKEN[229] = "atn"
    __TOKEN[199] = "auto"
    __TOKEN[191] = "call"
    __TOKEN[176] = "char"
    __TOKEN[237] = "chr$"
    __TOKEN[173] = "circle"
    __TOKEN[189] = "clear"
    __TOKEN[182] = "cload"
    __TOKEN[148] = "cls"
    __TOKEN[187] = "cont"
    __TOKEN[226] = "cos"
    __TOKEN[183] = "csave"
    __TOKEN[171] = "curmov"
    __TOKEN[170] = "curset"
    __TOKEN[145] = "data"
    __TOKEN[231] = "deek"
    __TOKEN[184] = "def"
    __TOKEN[147] = "dim"
    __TOKEN[138] = "doke"
    __TOKEN[172] = "draw"
    __TOKEN[129] = "edit"
    __TOKEN[200] = "else"
    __TOKEN[128] = "end"
    __TOKEN[225] = "exp"
    __TOKEN[164] = "explode"
    __TOKEN[240] = "false"
    __TOKEN[175] = "fill"
    __TOKEN[196] = "fn"
    __TOKEN[141] = "for"
    __TOKEN[218] = "fre"
    __TOKEN[190] = "get"
    __TOKEN[247] = "go"
    __TOKEN[155] = "gosub"
    __TOKEN[151] = "goto"
    __TOKEN[159] = "grab"
    __TOKEN[220] = "hex$"
    __TOKEN[158] = "himem"
    __TOKEN[162] = "hires"
    __TOKEN[153] = "if"
    __TOKEN[178] = "ink"
    __TOKEN[146] = "input"
    __TOKEN[215] = "int"
    __TOKEN[241] = "key$"
    __TOKEN[244] = "left$"
    __TOKEN[233] = "len"
    __TOKEN[150] = "let"
    __TOKEN[188] = "list"
    __TOKEN[142] = "llist"
    __TOKEN[224] = "ln"
    __TOKEN[232] = "log"
    __TOKEN[137] = "lores"
    __TOKEN[143] = "lprint"
    __TOKEN[246] = "mid$"
    __TOKEN[168] = "music"
    __TOKEN[193] = "new"
    __TOKEN[144] = "next"
    __TOKEN[202] = "not"
    __TOKEN[180] = "on"
    __TOKEN[210] = "or"
    __TOKEN[177] = "paper"
    __TOKEN[174] = "pattern"
    __TOKEN[230] = "peek"
    __TOKEN[238] = "pi"
    __TOKEN[166] = "ping"
    __TOKEN[169] = "play"
    __TOKEN[135] = "plot"
    __TOKEN[243] = "point"
    __TOKEN[185] = "poke"
    __TOKEN[134] = "pop"
    __TOKEN[219] = "pos"
    __TOKEN[186] = "print"
    __TOKEN[136] = "pull"
    __TOKEN[149] = "read"
    __TOKEN[131] = "recall"
    __TOKEN[160] = "release"
    __TOKEN[157] = "rem"
    __TOKEN[139] = "repeat"
    __TOKEN[154] = "restore"
    __TOKEN[156] = "return"
    __TOKEN[245] = "right$"
    __TOKEN[223] = "rnd"
    __TOKEN[152] = "run"
    __TOKEN[242] = "scrn"
    __TOKEN[214] = "sgn"
    __TOKEN[163] = "shoot"
    __TOKEN[227] = "sin"
    __TOKEN[167] = "sound"
    __TOKEN[197] = "spc"
    __TOKEN[222] = "sqr"
    __TOKEN[203] = "step"
    __TOKEN[179] = "stop"
    __TOKEN[130] = "store"
    __TOKEN[234] = "str$"
    __TOKEN[194] = "tab"
    __TOKEN[228] = "tan"
    __TOKEN[161] = "text"
    __TOKEN[133] = "troff"
    __TOKEN[132] = "tron"
    __TOKEN[239] = "true"
    __TOKEN[201] = "then"
    __TOKEN[195] = "to"
    __TOKEN[140] = "until"
    __TOKEN[217] = "usr"
    __TOKEN[235] = "val"
    __TOKEN[181] = "wait"
    __TOKEN[212] = "="
    __TOKEN[205] = "-"
    __TOKEN[207] = "/"
    __TOKEN[204] = "+"
    __TOKEN[206] = "*"
    __TOKEN[165] = "zap"
    __TOKEN[213] = "<"
    __TOKEN[211] = ">"
    __TOKEN[192] = "'"

    __REM_RE = "REM\s.*"
    def __read_basic_line(self):
        eof = False
        line = ""
        while True:
            c = self.__input_handle.read(1)
            if c=="":
                eof = True
                break
            oc = ord(c)
            if oc == 0:
                # end of line
                break
            elif oc < 0x80:
                if oc < 0x20:
                    c = " " # special char in comment e.g. home/blink
                line += c
            else:
                if oc in self.__TOKEN:
                    t = self.__TOKEN[oc].upper()
                    if t[0].isalnum() and len(line)>0:
                        last_char = line[-1]
                        if last_char.isalnum() or last_char in ['"',")"]:
                            line += " "
                    line += t
                    last_char = line[-1]
                    if last_char.isalnum():
                        line += " "
                else:
                    line += " <<unknown_%d>> " % oc
        return [eof,line]


    def __read_16bit_number(self):
        c1 = ord(self.__input_handle.read(1))
        c2 = ord(self.__input_handle.read(1))
        return c1 + (c2*256)

    def __read_header(self):
        c = ord(self.__input_handle.read(1))
        if c != 0x16:
            self.__error("Not a .TAP file (read 0x%02X instead of 0x16)" % c)

        while True:
            c = self.__input_handle.read(1)
            if c=="":
                self.__error("No data in file")
            oc = ord(c)
            if oc==0x24:
                break
            elif oc==0x16:
                pass
            else:
                self.__error("Corrupt header")
        h = self.__input_handle.read(9)

        tape_type = (ord(h[2])*256+ord(h[3]))

        self.__title = ""
        while True:
            c = self.__input_handle.read(1)
            if c=="":
                self.__error("Truncated .TAP file")
            oc = ord(c)
            if oc==0:
                break
            else:
                self.__title += c
        self.__message("Tape title: %s, tape type: %02X" % (python_lacks.ascii_compliant(self.__title),tape_type))
        return (tape_type == 0xC7 or tape_type == 0x00)

    def __doit(self):
        # main processing here
        self.__input_handle = open(self.__input_file,"rb")
        self.__output_handle = open(self.__output_file,"w")

        while not self.__read_header():
            self.__message("header read, filepos %d" % self.__input_handle.tell())
            while True:
                c = self.__input_handle.read(1)
                if c=="":
                    self.__error("End of file reached without encountering any BASIC file")
                elif ord(c)==0x16:
                    break

        last_valid_mem = 0
        while True:
            mem = self.__read_16bit_number()
            if (mem == 0):
                break
            last_valid_mem = mem

            line_number = self.__read_16bit_number()
            [eof,basic_line] = self.__read_basic_line()
            if not eof:
                self.__output_handle.write(str(line_number)+" "+basic_line+"\n")
        self.__message("Last line address: %04X, Last processed byte: %04X" % (last_valid_mem,self.__input_handle.tell()+0x501))
        self.__input_handle.close()
        self.__output_handle.close()

if __name__ == '__main__':
    """
        Description :
            Main application body
    """


    o = Tap2Bas()
    o._init_from_sys_args(debug_mode = True)
    #o.init("output_file")
