
import os,sys,re
import getopt,traceback,python_lacks
#import count_usage

GLOBAL_SCALAR_STRUCT = "gs"
GLOBAL_ARRAY_STRUCT = "gv"
GLOBAL_CONSTANT_POINTER = "cp"
TEMP_STRING_VAR = "str_temp"
DATA_ARRAY = "data_array"



class Bas2C:
    __VERSION_NUMBER = "1.2"
    try:
        __MODULE_FILE = __file__
    except:
        __MODULE_FILE = sys.executable
    __PROGRAM_NAME = os.path.basename(__MODULE_FILE)
    __PROGRAM_DIR = os.path.abspath(os.path.dirname(__MODULE_FILE))

    def __init__(self):
        self.__logfile = ""
        self.__temp_directory = None
        # init members with default values
        self.__case_statement = set()
        self.__output_directory = None
        self.__input_file = None
        self.generation_options = self.Options()

        self.__constid2value = dict()
        self.__value2constid = dict()  # reverse lookup table
        self.__constid_index = 0

        self.__scalar_variables = set()
        self.__unbounded_array_variables = set()
        self.__array_variables = dict()
        self.__forced_int_scalar = set()
        self.__forced_int_array = set()
        self.__data_array = list()
        self.__procedures = set()
        self.__static_asm_call = set()
        self._call_handler_needed = False
        self._goto_handler_needed = False
        self._gosub_handler_needed = False
        self.__scalar_input = False
        self._guessed_array_size = 128
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
                self._warn("Could not delete temp dir %s: %s" % (self.__temp_directory,output))

    def _init_from_sys_args(self,debug_mode = True):
        """ standalone mode """
        try:
            self.__do_init()
        finally:
            self.__delete_temp_directory()

 

    def __do_init(self):
        self.__parse_args()
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
            msg += os.linesep
        try:
            sys.stderr.write(msg)
            sys.stderr.flush()
        except:
            # ssh tunneling bug workaround
            pass
        if self.__logfile != "":
            f = open(self.__logfile,"a")
            f.write(msg)
            f.close()

    def __error(self,msg,user_error=True):
        """
        set user_error to False to trigger error report by e-mail
        """
        raise Exception("*** Error: "+msg+" ***")

    def _warn(self,line,msg):
        if line<0:
            li = "(global)"
        else:
            li = "Line %d" % line
        self.__message("*** Warning: %s:%s ***" % (li,msg))
    def _info(self,line,msg):
        if line<0:
            li = "(global)"
        else:
            li = "Line %d" % line
        self.__message("*** Info: %s:%s ***" % (li,msg))

    class Options:
        def __init__(self):
            self.floats_disabled = False
            self.logical_forced = False
            self.localize_strings = False
            self.clean_code = False
            self.mutate_floats = False

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
        ["case=","specify lines to insert a case (if --Clean-code)/label statement: start,stop,step (useful for dynamic goto/gosubs)"],
        ["Floats-disabled","consider all numeric variables as integers"],
        ["Logical-forced","do not try to figure out if AND & OR are logical (|) or boolean (||)"],
        ["localize-strings","redirect string constants to a table (allows easy translation)"],
        ["mutate-to-int","mutate floats involved in logical operations to int type"],
        ["Clean-code","suppose that the code does not have to many GOTOs everywhere and creates real C procedures"],
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
                self.__output_directory = value
            oi += 1
            if option in ('-c',options[oi]):
                self.__case_statement.add(value)
            oi += 1
            if option in ('-F',options[oi]):
                self.generation_options.floats_disabled = True
            oi += 1
            if option in ('-L',options[oi]):
                self.generation_options.logical_forced = True
            oi += 1
            if option in ('-l',options[oi]):
                self.generation_options.localize_strings = True
            oi += 1
            if option in ('-m',options[oi]):
                self.generation_options.mutate_floats = True
            oi += 1
            if option in ('-C',options[oi]):
                self.generation_options.clean_code = True

        if self.__output_directory == None:
            self.__error("output directory not set")
        if self.__input_file == None:
            self.__error("input file not set")

    def __usage(self):
        sys.stderr.write("Usage: "+self.__PROGRAM_NAME+self.__opt_string+os.linesep)
        sys.stderr.write(self.__opthelp)



    __FLOAT_RE = re.compile(r"([\+\-]?\d*\.\d*E?[\+\-]?\d+|[\+\-]?\d+E[\+\-]?\d+)")
    __INTEGER_RE = re.compile(r"([\+\-]?\d+|#[A-F0-9]+)",re.IGNORECASE)
    __VARIABLE_RE = re.compile(r"([A-Z_][0-9A-Z_]*[\$%]?)")
    __STRING_RE = re.compile(r'"([^"]*)"')
    __BAD_STRING_RE = re.compile(r'"([^"]*)$')

    __LINENUM_RE = re.compile(r"\s*(\d+)")
    __DATA_FIELD_RE = re.compile(r"([^,]*)")

    def __add_data_line(self,data_line):
        data_line = data_line.strip()
        pos=0
        while pos<len(data_line):
            m = self.__STRING_RE.match(data_line,pos)
            if m==None:
                # no quotes
                m = self.__DATA_FIELD_RE.match(data_line,pos)
                if m==None:
                    self.__error("Data parsing error pos %d: '%s'" % (pos,data_line))
                else:
                    d = m.group(1).strip()
            else:
                d = m.group(1)
            self.__data_array.append(d)

            pos = m.end()
            # advance to next comma
            while pos<len(data_line) and data_line[pos]!=',':
                pos+=1
            # skip comma
            pos+=1

    class Token:
        UNKNOWN = 0
        STRING = 1
        SCALAR_VARIABLE = 2
        ARRAY_VARIABLE = 3
        KEYWORD = 4
        INTEGER = 5
        HEX_INTEGER = 6
        FLOAT = 7
        def __init__(self,parent,value,ttype):
            if parent != None:
                self.line = parent._lineno
                self.column = parent._line_pos
                self.__nesting = parent._nesting
            self.value = value
            self.type = ttype
            self.children = [] # child tokens, filled later
            self.procedure = self.type == self.KEYWORD and value in Bas2C.BasicStatement._PROCEDURE_TOKENS
            self.function = self.type == self.KEYWORD and value in Bas2C.BasicStatement._FUNCTION_TOKENS

        def get_nesting(self):
            return self.__nesting

        def is_numeric_constant_type(self):
            return self.type in [self.INTEGER,self.HEX_INTEGER,self.FLOAT]

        def is_float_variable(self):
            return (self.type in [self.ARRAY_VARIABLE,self.SCALAR_VARIABLE] and self.value[-1] not in ["%","$"])

        def is_string_type(self):
            return isinstance(self.value,str) and (self.type == self.STRING or self.value[-1]=="$" or self.value == "SPC" or self.value.startswith("_STR"))

        def nest(self,delta=1):
            self.__nesting += delta
            for c in self.children:
                c.nest(delta)

        def nb_children(self,recursive=True):
            rval = len(self.children)
            if recursive:
                for c in self.children:
                    rval += c.nb_children()

            return rval

        def clone(self):
            rval = Bas2C.Token(None,self.value,self.type)
            rval.line = self.line
            rval.column = self.column # will be inexact if clone is inserted before/after this one
            rval.__nesting = self.__nesting
            return rval

        def __str__(self):
            if self.type == self.STRING:
                # escape '\' chars
                sv = self.value.replace("\\","\\\\")
                rval = '"'+str(sv)+'"'
            elif self.type == self.HEX_INTEGER:
                rval = "0x%X" % self.value
            else:
                rval = str(self.value)

            return rval


    class BasicStatement:
        def __init__(self,parent):
            """
            parent: Bas2C singleton
            """
            self.tokens=[]
            self.parent = parent
        def __str__(self):
            rval = ""
            for t in self.tokens:
                rval += str(t)
            return rval

        def _warn(self,line,message):
            self.parent._warn(line,message)
        def _info(self,line,message):
            self.parent._info(line,message)
        def __syntax_error(self,token,message):
            line_pos = token.column + len(str(token.line))+2 # adjust to take line number into account
            raw_line = self.__line(token.line).text
            i=0
            for i in range(line_pos,len(raw_line)):
                if raw_line[i]!=' ':
                    break
            line_pos = i
            marker = " "*line_pos+"^"
            raise Exception("parse error:\n%s\n%s\n%d:%d: %s" % (raw_line,marker,token.line,line_pos,message))

# utility methods
        def __line(self,lineno=-1):
            if lineno == -1:
                lineno = self.parent._lineno
            if lineno not in self.parent.lines_dict:
                raise Exception("Line %d does not exist" % lineno)
            return self.parent.lines_dict[lineno]

        def __goto_label(self,lineno):
            return "label_%d" % lineno
        def __gosub_procedure(self,lineno):
            return "BP_%05d" % lineno
        def __asm_procedure(self,address):
            return "AP_%04X" % address

        def __token_pos(self,tokens,tokpos,tok_value,exception_on_error=False):
            rval = -1
            i = tokpos

            while i < len(tokens):
                if tokens[i].value==tok_value:
                    rval = i
                    break
                i+=1

            if exception_on_error and rval == -1:
                self.__syntax_error(tokens[tokpos-1],"%s expected" % tok_value)

            return rval

        def __nb_tokens_to_end(self,tokens,tokpos):
            else_pos = self.__token_pos(tokens,tokpos,"ELSE")
            if else_pos == -1:
                # no else: discard to the end of the string
                skip=len(tokens)-tokpos
            else:
                # discard until "else"
                skip=else_pos-tokpos
            return skip

#############################
        # each __f_ method gets a t Token in the BasicStatement context
        # it must return a tuple [nb_consumed_tokens,c_token_list]
        # if nb_consumed_tokens == -1 then assume all statement tokens have been consumed (easier)

        def __f_discard(self,tokens,tokpos):
            # discard all remaining tokens from list until ELSE statement or end of line
            return [self.__nb_tokens_to_end(tokens,tokpos),[]]

        def __f_unsupported(self,tokens,tokpos):
            self.__syntax_error(tokens[tokpos-1],"unsupported token")

##[],#";\n}\nelse\n{\n",
        def __f_else(self,tokens,tokpos):
            # special case: "ELSE 10"
            rval = [";","\n","}\n","else","\n","{\n"]
            gskip = 0
            t = tokens[tokpos]
            # process ELSE 10 case grrrrr
            # only supported construction is integer. We omit (t.type == t.SCALAR_VARIABLE) to avoid false alarms
            # like THEN A=10, it will not compile and we'll fix it manually
            if (t.function and not t.value.startswith("_STR")) or t.value=='(' or t.type in [t.INTEGER,t.FLOAT]:
                [gskip,rval2] = self.__f_goto(tokens,tokpos)
                rval.extend(rval2)
                has_goto = 1
            return [gskip,rval]

        def __f_then(self,tokens,tokpos):
            # special case: "THEN 10"
            rval = []
            has_goto = 0
            t = tokens[tokpos]
            else_pos = self.__token_pos(tokens,tokpos,"ELSE")
            if tokpos==else_pos-1 or tokpos==len(tokens)-1: # last token
                # only supported construction is integer. We omit (t.type == t.SCALAR_VARIABLE) to avoid false alarms
                # like THEN A=10, it will not compile and we'll fix it manually
                if (t.function and not t.value.startswith("_STR")) or t.value=='(' or t.type in [t.INTEGER,t.FLOAT]:
                    [gskip,rval] = self.__f_goto(tokens,tokpos)
                    has_goto = 1
                    tokpos += gskip

            rval.extend(self.__translate_tokens(tokens,tokpos,else_pos))
            if else_pos == -1:
                # no else: process the end of the string
                skip=len(tokens)-tokpos
            else:
                # process until "else"
                skip=else_pos-tokpos
            # increase line nesting by 1
            self.__line().nesting += 1

            return [skip+has_goto,rval]

        def __f_if(self,tokens,tokpos):
            # scan until "then"
            then_pos = self.__token_pos(tokens,tokpos,"THEN",True)

            # scan tokens until "then"
            rval = ["if"," ","("]
            rval.extend(self.__translate_tokens(tokens,tokpos,then_pos))
            rval.extend([")\n","{\n"])
            return [then_pos-tokpos,rval]

        def __f_until(self,tokens,tokpos):
            rval = ["\n","}"," ","while","(","!"]
            # like C "while", but with condition reverted!
            rval.extend(self.__translate_tokens(tokens,tokpos))
            rval.append(")")
            return [-1,rval]

        def __f_dim(self,tokens,tokpos):
            rval=[]
            while tokpos<len(tokens):
                if tokens[tokpos].value==",":
                    pass
                else:
                    variable_name = tokens[tokpos].value
                    comma_pos = self.__token_pos(tokens,tokpos,",")
                    variable_size = self.__translate_tokens(tokens[tokpos].children)[1:-1]
                    reworked_variable_size = []
                    fixed_size = True
                    for v in variable_size:
                        if v==",":
                            reworked_variable_size.extend(["]","["])
                        else:
                            if v.isdigit():
                                reworked_variable_size.append(str(int(v)+1))
                            else:
                                fixed_size = False
                                reworked_variable_size.extend([v,"+","1"])

                    if fixed_size:
                        self.parent.add_array_variable(variable_name,reworked_variable_size)
                    else:
                        self._warn(tokens[tokpos].line,"Found dynamic array variable %s, setting size to %d, run-time check will be performed" % (variable_name,self.parent._guessed_array_size))
                        self.parent.add_array_variable(variable_name,[str(self.parent._guessed_array_size)])
                        if variable_name[-1]=="%":
                            elttype = "T_INT"
                            prefix = "int_"
                            vn = prefix+variable_name[-1]
                        elif variable_name[-1]=="$":
                            elttype = "T_STRING"
                            prefix = "str_"
                            vn = prefix+variable_name[-1]
                        else:
                            vn = variable_name
                            elttype = "T_NUMERIC"

                        cvn = GLOBAL_ARRAY_STRUCT+"."+vn
                        rval.extend(["dim_check","(",'"%s"' % vn, cvn,","]+reworked_variable_size+[",","sizeof("+cvn+")/sizeof("+elttype+")",";","\n"])
                    if comma_pos==-1:
                        break
                    tokpos = comma_pos
                tokpos+=1
            return [-1,rval]

        def __f_for(self,tokens,tokpos):
            #FOR I = 1 TO 3:  1,2,3: => for(i=1;i<=3;i++)
            #FOR I = A TO B STEP C:  for(i=A;(A<B) ? (i<=B) : (i>=B);i+=C)
            C_INTEGER_RE = re.compile(r"[\+\-]?\d+$")
            C_FLOAT_RE = re.compile(r"[\+\-]?\d+\.\d+$")
            rval = []
            iteration_variable = GLOBAL_SCALAR_STRUCT+"."+tokens[tokpos].value
            tokpos+=2
            to_pos = self.__token_pos(tokens,tokpos,"TO",True)
            start_value_expr = self.__translate_tokens(tokens,tokpos,to_pos)
            limit_expr = self.__translate_token(tokens,to_pos+1)
            step_pos = self.__token_pos(tokens,tokpos,"STEP")
            if (step_pos==-1): # no step
                increment_expr = ["1"]
            else:
                increment_expr = self.__translate_tokens(tokens,step_pos+1)

            is_simple_increment = (len(increment_expr)==1 and C_INTEGER_RE.match(increment_expr[0])) or \
            (len(increment_expr)==2 and increment_expr[0]=='-' and C_INTEGER_RE.match(increment_expr[1]))

            if is_simple_increment:
                increment_value = int("".join(increment_expr))
            else:
                is_simple_increment = (len(increment_expr)==1 and C_FLOAT_RE.match(increment_expr[0])) or \
                (len(increment_expr)==2 and increment_expr[0]=='-' and C_FLOAT_RE.match(increment_expr[1]))

                if is_simple_increment:
                    increment_value = float("".join(increment_expr))
            # common part
            rval.append("for (")
            rval.append(iteration_variable)
            rval.append("=")
            rval.extend(start_value_expr)
            rval.append(";")

            # simple case:
            if is_simple_increment:
                rval.extend(iteration_variable)
                if increment_value>0:
                    rval.append("<=")
                else:
                    rval.append(">=")
                rval.extend(limit_expr)
            else:
                # argh complex!
                rval.extend(["("]+start_value_expr+["<="]+limit_expr+[")","?","(",iteration_variable,"<="]+\
                limit_expr+[")",":","(",iteration_variable,">="]+limit_expr+[")"])
            # common part
            rval.append(";")
            rval.extend(iteration_variable)
            if increment_expr==["1"]:
                rval.extend("++")
            elif increment_expr==["-1"]:
                rval.extend("--")
            else:
                rval.extend("+=")
                rval.extend(increment_expr)

            rval.extend([")\n","{\n"])

            return [-1,rval]

        def __decode_linear_expression(self,tokens,tokpos,nb_toks_to_end):
            t = tokens[tokpos]
            if t.value=="(":
                # support with brackets: go inside
                tokpos = 0
                tokens = t.children
                nb_toks_to_end = len(tokens)-1
                t = tokens[0]

            start_line = None
            end_line = None
            step = None
            # find variable pos
            nb_variables = 0
            varpos = -1
            for i in range(tokpos,tokpos+nb_toks_to_end):
                if tokens[i].type in [t.ARRAY_VARIABLE,t.SCALAR_VARIABLE]:
                    varpos = i-tokpos
                    nb_variables+=1
            #if nb_variables==0 and nb_toks_to_end==3 and tokens[tokpos+1].value=="+":
            #    # 100 + (...
            #        start_line = int(t.value)

            if nb_variables==1:
                # we're going to try to extrapolate dynamic linear line
                # computation formula, usually like A + B * C where only one term varies
                # and generate the "case" labels automatically so the coder only has to insert the "switch"
                # we won't process if > 1: too complex
                if nb_toks_to_end==5:
                    if t.type in [t.INTEGER,t.FLOAT]:
                        op1 = tokens[tokpos+1]
                        op2 = tokens[tokpos+3]
                        if op1.value=="+" and op2.value=="*":
                            # 5: 4200 + 100 * S or (S * 100)
                            if varpos==4:
                                start_line = int(t.value)
                                step = int(tokens[tokpos+2].value)
                            elif varpos==2:
                                start_line = int(t.value)
                                step = int(tokens[tokpos+4].value)

                        elif op2.value=="+" and op1.value=="*":
                            # 5: 100 * S + 4200
                            if varpos==4:
                                step = int(t.value)
                                start_line = int(tokens[tokpos+2].value)
                            elif varpos==2:
                                step = int(t.value)
                                start_line = int(tokens[tokpos+4].value)
            elif nb_variables==0 and nb_toks_to_end==3:
                op1 = tokens[tokpos+1]
                if op1.value=="+" and tokens[tokpos+2].value=="(":
                    start_line = int(t.value)
                    children=tokens[tokpos+2].children
                    if len(children)==4 and children[1].value=="*":
                        ft = children[0]
                        st = children[2]
                        if ft.type in [t.INTEGER,t.FLOAT]:
                            step = int(ft.value)
                        elif st.type in [t.INTEGER,t.FLOAT]:
                            step = int(st.value)
            if start_line!=None and step!=None:
                if t.line == start_line:
                    # don't start at 0 because we're on this line
                    z = 1
                else:
                    z = 0
                start_z = -1
                while True:
                    candidate = start_line+step*z
                    if candidate in self.parent.lines_dict:
                        # valid line in expression: we suppose it's OK
                        end_line = candidate
                        if start_z==-1:
                            start_z = z
                    else:
                        # not found: don't necessarily break because start z could be wrong
                        if start_z!=-1 or candidate>65536:
                            break
                    z += 1

                # shift start_line according to actual start line
                start_line += start_z * step

            return [start_line,end_line,step]

        def __f_gosub(self,tokens,tokpos):
            t = tokens[tokpos]
            nb_toks_to_end = self.__nb_tokens_to_end(tokens,tokpos)
            if t.type in (t.INTEGER,t.FLOAT) and nb_toks_to_end==1:
                dest_line=int(t.value)
                self.__line(dest_line).gosub = True
                if self.parent.generation_options.clean_code:
                    gproc = self.__gosub_procedure(dest_line)
                    self.parent.add_procedure(gproc)
                    rval = [gproc,"(",")"]
                else:
                    # insert gosub macro
                    rval = ["GOSUB","(",str(dest_line),")"]
                nbskip=1
            else:
                self.parent._gosub_handler_needed = True
                # try to recognize dynamic patterns (most useful for GOSUB, not so great for GOTOs)
                [start_line,end_line,step] = self.__decode_linear_expression(tokens,tokpos,nb_toks_to_end)
                if self.parent.generation_options.clean_code:
                    if start_line != None and step != None:
                        self._warn(t.line,"unsupported variable/expression GOSUB address (try adding -c %d,%d,%d)" % (start_line,end_line,step))
                    else:
                        self._info(t.line,"unsupported variable/expression GOSUB address")
                else:
                    if start_line != None and end_line != None and step != None:
                        self._info(t.line,"variable/expression GOSUB address, adding GOSUB targets from %d to %d step %d" % (start_line,end_line,step))
                        # mark valid lines with "has_gosub" flag
                        for i in range(start_line,end_line,step):
                            self.__line(i).gosub = True
                    else:
                        e = " ".join(self.__translate_tokens(tokens,tokpos))
                        self._warn(t.line,"complex/undecoded variable/expression GOSUB address: "+e)

                rval = ["gosub_handler","("]
                gosub_expr = self.__translate_tokens(tokens,tokpos)
                nbskip=len(gosub_expr)
                rval.extend(gosub_expr)
                rval.extend([",",str(t.line),")"])
            return [nbskip,rval]


        def __f_goto(self,tokens,tokpos):
            t = tokens[tokpos]
            if t.type in [t.INTEGER,t.FLOAT] and self.__nb_tokens_to_end(tokens,tokpos)==1:
                dest_line=int(t.value)
                self.__line(dest_line).goto = True
                rval = ["goto"," ",self.__goto_label(dest_line)]
                nbskip=1
            else:
                self.parent._goto_handler_needed = True
                if self.parent.generation_options.clean_code:
                    self._warn(t.line,"unsupported variable/expression GOTO address")
                    rval = ["goto_handler","("]
                else:
                    self._info(t.line,"variable/expression GOTO address")
                    rval = ["GOTO","("]  # call to dynamic GOTO macro
                goto_expr = self.__translate_tokens(tokens,tokpos)
                nbskip = len(goto_expr)
                rval.extend(goto_expr)
                if self.parent.generation_options.clean_code:
                    rval.extend([",",str(t.line),")"])
                else:
                    rval.append(")")

            return [nbskip,rval]

        def __f_call(self,tokens,tokpos):
            t = tokens[tokpos]

            if t.type in [t.HEX_INTEGER,t.INTEGER] and t.value > 0xC000:
                # static ROM call: no need for dynamic: should be supported by run-time
                assembler_procedure = self.__asm_procedure(t.value)
                rval = [assembler_procedure,"(",")"]
                self.parent.add_procedure(assembler_procedure)
            else:
                else_pos = self.__token_pos(tokens,tokpos,"ELSE",False)
                if else_pos==-1:
                    else_pos = len(tokens)

                self.parent._call_handler_needed = True
                rval = ["call_handler","("]

                rval.extend(self.__translate_tokens(tokens,tokpos,end=else_pos))
                rval.extend([",",str(t.line),",","1",")"])
                if t.type in [t.HEX_INTEGER,t.INTEGER]:
                    self.parent.add_static_asm_call(t.value)
            return [-1,rval]


        def __f_on(self,tokens,tokpos):
            rval =["switch","(","(","T_INT",")"]
            go_pos = self.__token_pos(tokens,tokpos,"GOTO",False)
            gosub = go_pos==-1
            if gosub:
                # gosub?
                go_pos = self.__token_pos(tokens,tokpos,"GOSUB",True)

            value = self.__translate_tokens(tokens,tokpos,go_pos)
            rval.extend(value)
            rval.extend([")","\n","{"])
            targets = self.__translate_tokens(tokens,go_pos+1)
            i = 1
            for t in targets:
                if t != ",":
                    rval.extend(["\n","case"," ",str(i),":","\n"])
                    dest_line=int(t)
                    if gosub:
                        self.__line(dest_line).gosub = True
                        rval.extend([self.__gosub_procedure(dest_line),"(",")",";","\n"])
                    else:
                        self.__line(dest_line).goto = True
                        rval.extend(["goto"," ",self.__goto_label(dest_line),";","\n"])
                    i += 1
            rval.extend(["\n","default:","\n","break",";","\n","}"])
            return [-1,rval]

        def __f_next(self,tokens,tokpos):
            rval = []
            if tokpos==1:
                # count how many variables the NEXT supports
                nb_vars = self.__nb_tokens_to_end(tokens,tokpos-1) // 2
                if nb_vars==0:
                    nb_vars=1
                for i in range(0,nb_vars):
                    rval.extend(["\n","/* NEXT */","\n","}","\n"])
            elif tokens[tokpos-2].value=="THEN":
                rval.append("continue")
                # warn the user: translator is not powerful enough to rework THEN NEXT: other instructions into a
                # valid C for loop with end condition
                # THEN NEXT without anything is easily emulated by a "continue", though.
                self._warn(tokens[tokpos-1].line,"THEN NEXT: inserted continue, but code may need rewrite/added ')'")
            return [-1,rval]

        def __f_print(self,tokens,tokpos):
            if tokpos==len(tokens):
                # no params: end of line
                return[-1,["print","(","NULL",")"]]
            if tokens[tokpos]=="@":
                # special print@
                raise Exception("unsupported print@ ATM")
            else:
                return self.__f_print_or_input(tokens,tokpos,"print")

        def __f_plot(self,tokens,tokpos):
            rval = ["plotva","("]
            # coords
            rval.extend(self.__translate_token(tokens,tokpos+1))
            rval.append(",")
            rval.extend(self.__translate_token(tokens,tokpos+3))
            rval.append(",")
            else_pos = self.__token_pos(tokens,tokpos,"ELSE")
            if else_pos==-1:
                else_pos=len(tokens)
            i = tokpos+5
            while i < else_pos-1:
                t = tokens[i]
                if t.type == t.KEYWORD and t.value==",":
                    rval.append(",")
                else:
                    openparen=False
                    if not t.is_string_type():
                        openparen=True
                        rval.extend(["chr$","("])  # convert to string first
                    # simplest case string, string variable or string function: inject as-is
                    rval.extend(self.__translate_token(tokens,i))
                    if openparen:
                        rval.append(")")
                i+=1
            if else_pos==8:
                rval[0]="plot"
                rval.append(")")
            else:
                rval.extend([",","NULL",")"])

            return [-1,rval]

        def __f_input(self,tokens,tokpos):
            return self.__f_print_or_input(tokens,tokpos,"input")

        def __f_print_or_input(self,tokens,tokpos,function_name):
            PRINT_OPERATORS = ["*","/","+","-","("]
            rval = [function_name,"("]
            null_terminator = ["NULL",")"]
            else_pos = self.__token_pos(tokens,tokpos,"ELSE")
            if else_pos==-1:
                else_pos=len(tokens)

            last_token_is_semicolon=False
            first_arg = True
            i = tokpos
            while i < else_pos:

                t = tokens[i]
                last_token_is_semicolon = t.value==";"
                if last_token_is_semicolon:
                    # skip
                    pass
                else:
                    if first_arg:
                        first_arg = False
                    else:
                        rval.append(",")

                    # consider argument
                    if t.is_string_type():
                        # simplest case string, string variable or string function: inject as-is
                        rval.extend(self.__translate_token(tokens,i))
                    elif t.value=="TAB":
                        # special case
                        rval.extend(["PRINT_TAB_SEPARATOR",",(const char*)("])
                        i+=1
                        rval.extend(self.__translate_token(tokens,i))
                        rval.append(")")
                    elif t.type in [t.INTEGER,t.HEX_INTEGER,t.FLOAT,t.SCALAR_VARIABLE,t.ARRAY_VARIABLE] or t.function:
                        # integer,float or function result (non-string as ruled out by condition above)
                        # inject, but wrapped by str$() function
                        # problem is print format is permissive: PRINT "hello";A*2 => we want to scan tokens until operator/parenthesis/non-string variable
                        last_i = i+1
                        while last_i<len(tokens):
                            nt = tokens[last_i]
                            if nt.value == "ELSE" or nt.value in PRINT_OPERATORS or nt.type in [t.INTEGER,t.HEX_INTEGER,t.FLOAT,t.SCALAR_VARIABLE,t.ARRAY_VARIABLE] or nt.function:
                                last_i+=1
                            else:
                                break
                        arg = self.__translate_tokens(tokens,i,last_i)
                        i = last_i-1
                        if function_name == "input":
                            self.parent.set_scalar_input(True)
                            # we inject a temporary string and parse it later
                            rval.append(TEMP_STRING_VAR)
                            null_terminator.extend([";","\n"]+arg)
                            if t.value[-1]=="%":
                                conversion_function = 'atoi'
                            else:
                                conversion_function = 'atof'
                            null_terminator.extend(["=",conversion_function,"(",TEMP_STRING_VAR,")"])
                        else:
                            # integer,float or function result (non-string as ruled out by conditions above)
                            # inject, but wrapped by str$() function
                            rval.extend(["str$","("])
                            rval.extend(arg)
                            rval.append(")")
                    elif t.value in ["+","/","*","-"]:
                        rval.append(t.value)
                    elif t.value==",":
                        # insert special separation code
                        rval.append("PRINT_COMMA_SEPARATOR")
                    else:
                        self.__syntax_error(t,"unsupported PRINT parameter %s" % t.value)
                i+=1
            if last_token_is_semicolon:
                if rval[-1]==",":
                    rval.pop()
            else:
                # no semicolon: linefeed
                if function_name != "input":
                    rval.extend([",","PRINT_LINEFEED"])
            if rval[-1]!="(":
                rval.append(",")
            rval.extend(null_terminator)
            return [else_pos-tokpos,rval]


        def __f_read(self,tokens,tokpos):
            nb_data = (len(tokens)-tokpos)//2+1
            rval = ["data_read","(",DATA_ARRAY,",","total_nb_data",",",str(nb_data),","]
            for i in range(tokpos,len(tokens)):
                t = tokens[i]
                if t.value==',':
                    pass
                else:
                    if t.value[-1]=="%":
                        rval.append("DT_INT")
                        rval.append(",&")
                    elif t.value[-1]=="$":
                        rval.append("DT_STRING")
                        rval.append(",")
                    else:
                        rval.append("DT_NUMERIC")
                        rval.append(",&")

                    rval.extend(self.__translate_token(tokens,i))
                    rval.append(",")
            rval[-1]=")"

            return [-1,rval]

        def __f_run(self,tokens,tokpos):
            nb_vars = self.__nb_tokens_to_end(tokens,tokpos-1)
            t = tokens[tokpos+1]

            if t.value==")":
                # no params: goto start of program
                ld = sorted(self.parent.lines_dict.keys())
                dest_line = ld[0]
            else:
                if t.type == t.INTEGER:
                    dest_line = t.value
                else:
                    self.__syntax_error(t,"unsupported dynamic RUN parameter")

            self.__line(dest_line).goto = True
            rval = ["CLEAR","(",")",";","goto"," ",self.__goto_label(dest_line)]
            return [nb_vars,rval]

        __COMPARISON_OPS = ["=","<>",">","<","<=",">="]

        def __logical_or_boolean_guess(self,tokens,tokpos):
            if self.parent.generation_options.logical_forced:
                return True  # no risks, slower, and strange for a C coder but always good at least!

            t= tokens[tokpos]
            if t.function and t.value.startswith("_STR"):
                # string comparison functions: boolean for sure!
                return False

            # recursively analyse expression and try to tell if it's logical or boolean OR/AND
            logical=True
            then_pos =  self.__token_pos(tokens,tokpos,"THEN")
            if then_pos == -1:
                nb_vars = self.__nb_tokens_to_end(tokens,tokpos)
            else:
                nb_vars=then_pos-tokpos

            if nb_vars==1:   #OR (something between parentheses)
                # probably logical in all cases: recurse to child and see if child is a logical condition
                if len(tokens[tokpos].children)>0:
                    logical=self.__logical_or_boolean_guess(tokens[tokpos].children,0)
            else:
                t = tokens[tokpos+1]
                if t.value in self.__COMPARISON_OPS:  # cond OR/AND X=2 => boolean
                    logical=False
                elif t.value in ["AND","OR"]:  # compound expression with multiple AND/OR
                    if len(tokens[tokpos].children)>0:
                        logical=self.__logical_or_boolean_guess(tokens[tokpos].children,0)
                elif tokens[tokpos].value=="NOT":  # negation
                    if len(tokens[tokpos+1].children)>0:
                        logical=self.__logical_or_boolean_guess(tokens[tokpos+1].children,0)

            return logical

        def __mutate_operands(self,tokens,tokpos):
            # tokpos-1: position of "OR"/"AND"
            left = tokens[tokpos-2]
            right = tokens[tokpos]
            if left.is_float_variable():
                # mutate: change from float to integer
                self.parent.mutate_to_int(left)
            if right.is_float_variable():
                self.parent.mutate_to_int(right)
                pass

        def __f_and(self,tokens,tokpos):
            # && or & depending on boolean or logical operation
            # IF A = 0 AND B = 0 => &&
            # A AND B => &

            if self.__logical_or_boolean_guess(tokens,tokpos):
                rval = [' & ']
                self.__mutate_operands(tokens,tokpos)
            else:
                rval = [" && "]
            return [0,rval]

        def __f_pop(self,tokens,tokpos):
            if self.parent.generation_options.clean_code:
                self.__f_unsupported(tokens,tokpos)
            else:
                # just insert POP macro
                rval = ["POP"];
            return [0,rval]

        def __f_stop(self,tokens,tokpos):
            rval = ["error","(",'"STOP encountered line %d"' % tokens[tokpos-1].line,",","NULL",")"]
            return [0,rval]

        def __f_or(self,tokens,tokpos):
            # try to figure out if OR boolean or logical operation
            #OR A=12 => boolean
        	#OR A <else/end> => logical
            # if not possible to know, then go for logical (less risks)
            # IF A = 0 OR B = 0 => ||
            # A OR B => |
            if self.__logical_or_boolean_guess(tokens,tokpos):
                rval=[" | "]
                self.__mutate_operands(tokens,tokpos)
            else:
                rval=[" || "]

            return [0,rval]

        def __f_fre(self,tokens,tokpos):
            rval = ["48000"]  # simulates 48k free / garbage collection
            return [-1,rval]

        def __f_equals(self,tokens,tokpos):
            # equality/affectation for everything but string

            #print "FEQUALS",tokens[tokpos].value,self.__dump_tree_r(tokens[tokpos].children,False)
            if tokens[tokpos-1].get_nesting()==0 and (tokpos == 2 or tokens[tokpos-3].value in ["LET","THEN","FOR","ELSE"]):
                # affectation
                rval = [' = ']
            else:
                # test
                rval = [" == "]
            return [0,rval]

        def __f_str_equals(self,tokens,tokpos):
            # equality/affectation for strings
            rval = []

            #print "FEQUALS",tokens[tokpos].value,self.__dump_tree_r(tokens[tokpos].children,False)
            if tokens[tokpos-1].get_nesting()==0 and (tokpos == 1 or tokens[tokpos-2].value in ["LET","THEN","FOR","ELSE"]):
                # affectation
                rval = ["strcopy"]  # we don't reuse strcpy to avoid that generator mixes "==" with "=". strcopy returns void for that!
            else:
                # test
                rval = ["streq"]
            rval.extend(self.__translate_tokens(tokens[tokpos-1].children))
            return [0,rval]


        # None: same value lowercase / unexploited
        # empty string: ignore
        # another string: function name
        # python method: special case

        _KEYWORDS_DICT = {"ABS" : None,
        "AND" : __f_and,
        "ASC" : "asc",
        "ATN" : "atan",
        "AUTO" : __f_unsupported,
        "CALL" : __f_call,
        "CHAR" : "hires_draw_char",
        "CHR$" : None,
        "CIRCLE" : None,
        "CLEAR" : "CLEAR",
        "CLOAD" : __f_unsupported,
        "CLS" : None,
        "CONT" : __f_unsupported,
        "COS" : None,
        "CSAVE" : __f_unsupported,
        "CURMOV" : None,
        "CURSET" : None,
        "DATA" : None,
        "DEEK" : None,
        "DEF" : __f_unsupported,
        "DIM" : __f_dim,
        "DOKE" : None,
        "DRAW" : None,
        "EDIT" : __f_unsupported,
        "ELSE" : __f_else,
        "END" : None,
        "EXP" : None,
        "EXPLODE" : None,
        "FALSE" : "ORIC_FALSE",
        "FILL" : None,
        "FN" : __f_unsupported,
        "FOR" : __f_for,
        "FRE" : __f_fre,
        "GET" : None,
        "GOSUB" : __f_gosub,
        "GOTO" : __f_goto,
        "GRAB" : __f_discard,
        "HEX$" : None,
        "HIMEM" : __f_discard,
        "HIRES" : None,
        "IF" : __f_if,
        "INK" : None,
        "INPUT" : __f_input,
        "INT" : "integer",
        "KEY$" : "key$()",
        "LEFT$" : None,
        "LEN" : "strlength",
        "LET" : "",
        "LIST" : __f_unsupported,
        "LLIST" : __f_unsupported,
        "LN" : "log",
        "LOG" : "log10",
        "LORES" : None,
        "LPRINT" : __f_unsupported,
        "MID$" : None,
        "MUSIC" : None,
        "NEW" : __f_unsupported,
        "NEXT" : __f_next,
        "NOT" : "~",  # by default: arithmetic NOT, not logical not
        "ON" : __f_on,
        "OR" : __f_or,
        "PAPER" : None,
        "PATTERN" : None,
        "PEEK" : None,
        "PI" : "M_PI",
        "PING" : None,
        "PLAY" : None,
        "PLOT" : None,
        "POINT" : None,
        "POKE" : None,
        "POP" : __f_pop,
        "POS" : None,
        "PRINT" : __f_print,
        "?" : __f_print,
        "PULL" : __f_discard,
        "READ" : __f_read,
        "RECALL" : __f_unsupported,
        "RELEASE" : __f_discard,
        "REM" : None,
        "REPEAT" : ["do","{\n"],
        "RESTORE" : None,
        "RETURN" : None,
        "RIGHT$" : None,
        "RND" : None,
        "RUN" : __f_run,
        "SCRN" : None,
        "SGN" : None,
        "SHOOT" : None,
        "SIN" : None,
        "SOUND" : None,
        "SPC" : None,
        "SQR" : "sqrt",
        "STEP" : None,
        "STOP" : __f_stop,
        "STORE" : __f_unsupported,
        "STR$" : None,
        "TAB" : None,
        "TAN" : None,
        "TEXT" : None,
        "THEN" : __f_then,
        "TO" : None,
        "TROFF" : __f_discard,
        "TRON" : __f_discard,
        "TRUE" : "ORIC_TRUE",
        "UNTIL" : __f_until,
        "USR" : __f_unsupported,
        "VAL" : None,
        "WAIT" : "oric_wait",
        "ZAP" : None,
        "=<" : None,
        ">=" : None,
        "<>" : "!=",
        "&" : __f_unsupported,
        "!" : __f_unsupported,
        "+" : None,
        "-" : None,
        "*" : None,
        "/" : None,
        "^" : None,
        "<" : None,
        "<=" : None,
        ">=" : None,
        "=" : __f_equals,
        "'" : None,
        "(" : None,
        ")" : None,
        ";" : "",
        ":" : None,
        "," : None,
        ">" : None,
        # fake basic functions inserted by "fix_tree" method to make
        # conversion to C easier
        "_MID2$" : "mid2$",  # MID$ with only 2 parameters
        "_PLOT_CHAR" : "plot_char",
        "_STR_EQUALS" : __f_str_equals,
        "_STR_NOT_EQUALS" : "strneq",
        "_STR_CONCAT" : "strconcat",
        "_LOOP_WAIT" : "loopwait",
        "_POW" : "pow"
                }

        # procedure needing () when translated to C function calls
        _PROCEDURE_TOKENS = set(["CIRCLE","CLEAR","CLOAD","CHAR","CLS","CONT","CSAVE","CURMOV","CURSET","DEF","DOKE","DRAW","EDIT","END",
"FILL","FN","GRAB","HIMEM","HIRES","INK","LORES","MUSIC","NEW","PAPER","PATTERN","PLAY","PLOT","POINT","POKE","POP","PULL","GET","CALL"
"RECALL","RELEASE","RESTORE","GET","RUN","LIST","LLIST","SOUND","STORE","TEXT","UNTIL","USR","WAIT","ZAP","PING","EXPLODE","SHOOT","_LOOP_WAIT"]) # ,"GOTO","GOSUB"
        _FUNCTION_TOKENS = set(["ABS","ASC","ATN","CHR$","COS","DEEK","EXP","FRE","HEX$",
"INT","LEFT$","LEN","LN","LOG","LPRINT","MID$","PEEK","POS",
"RIGHT$","RND","SCRN","SGN","SIN","SPC","SQR","STR$","TAB","TAN","VAL",
"_STR_CONCAT","_MID2$","_POW"]) # specially created functions

        def __translate_tokens(self,tokens,start=0,end=-1,debug=False):
            rval = []
            tokpos = start
            max_tokpos = end

            if max_tokpos==-1:
                max_tokpos=len(tokens)
            while tokpos < max_tokpos:
                t = tokens[tokpos]
                if debug:
                    print("TT=>",tokpos,t.value)
                if t.type == t.KEYWORD:
                    v = self._KEYWORDS_DICT[t.value]
                    if v==None:
                        c_value = t.value.lower()
                        rval.append(c_value)
                        #if t.value in self._PROCEDURE_TOKENS
                        if len(t.children)>0:  # '('
                            rval.extend(self.__translate_tokens(t.children,debug=debug))
                    elif v=="":
                        pass
                    elif isinstance(v,list):
                        c_value = v
                        rval.extend(c_value)
                        if len(t.children):  # '('
                            rval.extend(self.__translate_tokens(t.children,debug=debug))
                    elif isinstance(v,str):
                        c_value = v
                        rval.append(c_value)
                        if len(t.children):  # '('
                            rval.extend(self.__translate_tokens(t.children,debug=debug))
                    else:
                        # function call

                        r = v(self,tokens,tokpos+1)
                        if r == None:
                            self.__f_unsupported(tokens,tokpos+1)
                        [nb_skip,c_tokens] = r
                        if nb_skip==-1:
                            nb_skip=len(tokens)-tokpos
                        tokpos+=nb_skip
                        rval.extend(c_tokens)
                else:
                    string_value = self.parent.get_token_repr(t)

                    rval.append(string_value)
                    if t.type == t.ARRAY_VARIABLE:
                        params = t.children

                        # we have to replace () by []
                        nt = params[0] #tokens[tokpos+1]
                        if nt.value!="(":
                            self.__syntax_error(nt,"Array variable %s should be followed by (" % t.value)
                        else:
                            j = 1
                            while j < len(params)-1:
                                next_j = j
                                while True:
                                    if next_j == len(params)-1 or params[next_j].value in [",",")"]:
                                        break
                                    next_j+=1

                                rval.append("[")

                                ttoks = self.__translate_tokens(params,j,next_j)
                                integer_cast = True
                                if len(ttoks)==1 and ttoks[0].isdigit():
                                    # integer: direct evaluation
                                    index=int(ttoks[0])  # was -1
                                    rval.append(str(index))
                                else:
                                    if params[j].value[-1]!="%":
                                        rval.append("(T_INT)")
                                    # not integer: add -1 suffix to correct BASIC array offset,
                                    # but try to reduce in case of final substract/add of an integer
                                    # to avoid clumsy C code
##                                    if len(ttoks)>1 and ttoks[-2]=="+":
##                                        if ttoks[-1]=="1":
##                                            # + 1 => remove
##                                            ttoks = ttoks[:-2]
##                                        elif ttoks[-1].isdigit():
##                                            # + number: => + prev number
##                                            ttoks[-1] = str(int(ttoks[-1])-1)
##                                    elif len(ttoks)>1 and ttoks[-2]=="-":
##                                        if ttoks[-1].isdigit():
##                                            # - number: => - next number
##                                            ttoks[-1] = str(int(ttoks[-1])+1)
##                                    else:
##                                        # can't simplify: just add -1 to expression
##                                        ttoks.extend(["-","1"])
                                    rval.extend(ttoks)
                                rval.append("]")
                                j = next_j+1
                tokpos+=1
            return rval


        def __translate_token(self,tokens,tokpos):
            return self.__translate_tokens(tokens,start=tokpos,end=tokpos+1)

        def translate_to_c(self):
            """
            translate current statement to C
            """
            rval = self.__translate_tokens(self.children)
            return rval

        def compute_tree(self):
            """
            compute "tree" according to nesting
            uses self.tokens as input
            creates self.children as output
            """
            self.children = self.__compute_tree_r(start_index=0,nesting=0)

        def __insert_token(self,token,new_token,after=False,after_children=True):
            """
            insert new_token before/after token in flat tokens list
            (will need tree recomputation in the end)
            """
            i = self.tokens.index(token)
            if after:
                if after_children:
                    i += self.tokens[i].nb_children()  # after = after last child of this token
                i += 1

            self.tokens.insert(i,new_token)

        def __delete_token(self,token,with_children=True):
            """
            insert new_token before token in flat tokens list
            (will need tree recomputation in the end)
            """
            if with_children:
                for t in token.children:
                    self.__delete_token(t)

            i = self.tokens.index(token)
            self.tokens.pop(i)



        def fix_tree(self):
            """
            If token is a procedure, insert parenthesis + increase nesting until next "ELSE" or
            statement end where it insert a closing parenthesis, recompute tree if needed

            Using the existing nesting, fix the following constructions by inserting
            pseudo basic functions in the flat token list (C cannot handle POW nor
            string concatenation/affectation/comparison through operators)

            Then, we'll have to recompute the tree if something has been done

            A$ + "string" + ... => _STRCONCAT(A$,"string",...)
            X ^ Y => _POW(X,Y)
            B$ = "hello" (affectation) => _STRCPY("hello",B$)
            B$ = "hello" (comparison after IF or UNTIL) => _STRCMP("hello",B$)

            """
            #print "TREE",self.__dump_tree(with_levels=True) #temp debug
            needs_recompute = False

            i = 0

            # flat scan for procedures parenthesis wrapping
            while i < len(self.tokens):
                t = self.tokens[i]
                if t.procedure:
                    needs_recompute = True
                    # insert a parenthesis
                    open_paren = t.clone()
                    open_paren.value = "("
                    open_paren.procedure = False
                    self.__insert_token(t,open_paren,after=True)
                    # forward until end of statement/else and increase nesting
                    # in the process

                    i += 1 # skip the newly inserted paren
                    tj = t # in case of no params, should insert ()
                    insert_after = True
                    while i < len(self.tokens):
                        tj = self.tokens[i]
                        if tj.value=="ELSE":
                            insert_after=False
                            break
                        i += 1

                    close_paren = t.clone()
                    close_paren.value = ")"
                    close_paren.type = t.KEYWORD
                    close_paren.procedure = False
                    self.__insert_token(tj,close_paren,after=insert_after)
                i += 1

            if needs_recompute:
                self.compute_tree()

            # tree scan to convert string additions => concat function

            needs_recompute = self.__fix_tree_r(self.children,pass_number=1)

            if needs_recompute:
                self.compute_tree()
                #print "TREE PASS1",self.__dump_tree(with_levels=True) #temp debug

            # tree scan for POW and string equals/affectations
            needs_recompute = self.__fix_tree_r(self.children,pass_number=2)

            if needs_recompute:
                self.compute_tree()
                #print "TREE PASS2",self.__dump_tree(with_levels=True) #temp debug

            # tree scan for POW and string equals/affectations
            while self.__fix_tree_r(self.children,pass_number=3):
                self.compute_tree()

        def __fix_tree_r(self,children,pass_number):
            """
            examine tree and insert in flat list, then recompute tree
            """
            rval = False
            i = 0
            while i < len(children):
                t = children[i]
                if pass_number==1:
                    if t.value == "MID$" and len(t.children)==5:
                        # 2 params only: replace function name
                        t.value = "_MID2$"
                    elif t.value == "PLOT":
                        nb_coma = 0
                        for j in range(i+2,len(children)):
                            if children[j].type == t.KEYWORD and children[j].value==",":
                                nb_coma+=1
                            elif nb_coma==2:
                                if not children[j].is_string_type():
                                    # last param is a non-string: replace function name
                                    t.value = "_PLOT_CHAR"
                                break
                if pass_number==3 and t.value=="(":
                    # simplify (number) => number (next 3rd pass will keep on computing)
                    if len(t.children)==2 and t.children[0].is_numeric_constant_type():
                        t.type = t.children[0].type
                        t.value = t.children[0].value
                        t.children = []
                        rval = True

                elif i>0 and i<len(children)-1:
                    previous_t = children[i-1]
                    next_t = children[i+1]
                    if pass_number==3:
                        if t.value in ["/","*","+","-","AND","OR"]:
                            a = previous_t.value
                            b = next_t.value
                            c = None
                            d = None
                            if i<len(children)-2:
                                c = children[i+2].value  # next operator if exists
                            if i>1:
                                d = children[i-2].value  # previous operator if exists
                            #print "SLIDING",d,a,t.value,b,c
                            # evaluate constant expressions like 2*5, 6/7 ...
                            # problem: if not done at this level, C division on integers will be problematic
                            # with constructions like 3/2: in C: 0 in BASIC: 1.5 (and in future python too!!)

                            if previous_t.is_numeric_constant_type() and next_t.is_numeric_constant_type():
                                result = None
                                if t.value=="/":
                                    # reduce
                                    if previous_t.type == t.FLOAT or next_t.type == t.FLOAT:
                                        # normal division
                                        if b==0:
                                            self.__syntax_error("Division by zero",t)
                                        result = a / b
                                        result_type = t.FLOAT
                                    else:
                                        # integer division, but should be float if not integer
                                        if a % b == 0:
                                            result = a // b
                                            result_type = t.INTEGER
                                        else:
                                            result = float(a)/b
                                            result_type = t.FLOAT
                                else:
                                    # other operators: standard computation
                                    if previous_t.type == t.FLOAT or next_t.type == t.FLOAT:
                                        result_type = t.FLOAT
                                    else:
                                        result_type = t.INTEGER
                                    if t.value=="*" and c != "/" and d != "/":
                                        result = a * b
                                    elif t.value=="+" and c not in ["*","/"] and d not in ["*","/"]:
                                        result = a + b
                                    elif t.value=="-" and c not in ["*","/"] and d not in ["*","/"]:
                                        result = a - b
                                    elif t.value=="AND":
                                        result = a & b
                                    elif t.value=="OR":
                                        result = a | b

                                rval = result != None
                                if rval:
                                    # skip 2 next children (not deleted yet in tree)
                                    i += 2

                                    previous_t.value = result
                                    previous_t.type = result_type
                                    # remove operator and right term now that it's solved
                                    self.__delete_token(t)
                                    self.__delete_token(next_t)

                    elif pass_number==2:
                        if previous_t.is_string_type():
                            if t.value in ["=","<>"]:

                                # convert "=" and "<>" to functions for strings
                                eq_token = t.clone()
                                if t.value=="=":
                                    eq_token.value="_STR_EQUALS"
                                else:
                                    eq_token.value="_STR_NOT_EQUALS"
                                rval = True

                                previous_t.nest()
                                next_t.nest()
                                t.nest()
                                t.value = ","

                                eq_token.function = True

                                # non-C compatible operator: replace expression by function call: e1 ^ e2 => _POW(e1,e2)
                                self.__insert_token(previous_t,eq_token,after=False)
                                open_paren = t.clone()
                                open_paren.value = "("
                                self.__insert_token(eq_token,open_paren,after=True)
                                close_paren = t.clone()
                                close_paren.value = ")"
                                self.__insert_token(next_t,close_paren,after=True)
                    elif pass_number==1:

                        if t.type == t.KEYWORD and t.value=="^":
                            rval = True

                            t.value = ","
                            pow_token = t.clone()
                            t.nest()
                            previous_t.nest()
                            next_t.nest()
                            pow_token.value = "_POW"
                            pow_token.function = True
                            # non-C compatible operator: replace expression by function call: e1 ^ e2 => _POW(e1,e2)
                            self.__insert_token(previous_t,pow_token,after=False)
                            open_paren = t.clone()
                            open_paren.value = "("
                            self.__insert_token(pow_token,open_paren,after=True)
                            close_paren = t.clone()
                            close_paren.value = ")"
                            self.__insert_token(next_t,close_paren,after=True)

                        elif previous_t.is_string_type() and t.value == "+":
                            # string concatenation: is it the first one?
                            if i<2 or children[i-2].value!="," or not children[i-3].is_string_type():  # "+" has already become ","
                                # first one
                                cat_token = t.clone()
                                cat_token.value="_STR_CONCAT"

                                rval = True
                                t.value = ","
                                t.nest()
                                previous_t.nest()
                                next_t.nest()
                                cat_token.function = True
                                # non-C compatible operator: replace expression by function call: e1 ^ e2 => _POW(e1,e2)
                                self.__insert_token(previous_t,cat_token,after=False)
                                open_paren = t.clone()
                                open_paren.value = "("
                                self.__insert_token(cat_token,open_paren,after=True)
                            else:
                                # next one
                                t.value = ","
                                t.nest()
                                next_t.nest()

                            if i>len(children)-3 or children[i+2].value!="+":
                                # no more
                                comma_arg = t.clone()
                                comma_arg.value = ","
                                self.__insert_token(next_t,comma_arg,after=True)
                                zero_arg = t.clone()
                                zero_arg.value = "NULL"
                                zero_arg.type = t.INTEGER
                                self.__insert_token(comma_arg,zero_arg,after=True)
                                close_paren = t.clone()
                                close_paren.value = ")"
                                self.__insert_token(zero_arg,close_paren,after=True)


                if len(t.children) >0:
                    if self.__fix_tree_r(t.children,pass_number):
                        rval = True

                i+=1
            return rval

        def __dump_tree(self,with_levels):
            return self.__dump_tree_r(self.children,with_levels)

        def __dump_tree_r(self,children,with_levels):
            rval = ""
            for t in children:
                rval += str(t.value)
                if with_levels:
                    rval += "[%d]" % t.get_nesting()
                rval +=" "
                if len(t.children):
                    if with_levels:
                        rval += " children <"
                    rval += self.__dump_tree_r(t.children,with_levels)
                    if with_levels:
                        rval += ">"

            return rval

        def __compute_tree_r(self,start_index,nesting):
            tokens = self.tokens
            nb_tokens = len(tokens)
            children = []
            i = start_index
            while i < nb_tokens:
                t = tokens[i]
                if t.get_nesting()==nesting:
                    children.append(t)
                    t.children = []
                    if i<nb_tokens-1 and tokens[i+1].get_nesting()>nesting:
                        t.children = self.__compute_tree_r(start_index=i+1,nesting=nesting+1)
                        i += t.nb_children()  # => also count recursively!!!
                else:
                    break
                i+=1
            return children



    class BasicLine:
        def __init__(self,parent,line,text):
            self.goto = False
            self.gosub = False
            self.case_statement = False
            self.nesting = 0
            self.line = line
            self.comment = None
            self.statements = []
            self.ret = False
            self.text = text
            self.parent = parent
            self.__c_tokens = []

        def has_sequence_break(self):
            rval = False
            # True if last statement of the Line is an instruction forbidding the program to continue on next line
            if len(self.statements)>0:
                last_statement = self.statements[-1]  # check only last statement (could happen on others but would have no sense)
                if len(last_statement.tokens)>0:
                    lst =  last_statement.tokens[0]
                    rval = lst.type == lst.KEYWORD and lst.value in ["RETURN","GOTO","RUN","STOP","END"]
            return rval

        def __str__(self):
            rval = str(self.line)+" "
            for s in self.statements:
                rval += str(s)+":"
            rval = rval.rstrip(":")
            if self.comment:
                rval += "' " + self.comment
            return rval

        def translate_to_c_first_pass(self):
            self.__c_tokens = []
            for s in self.statements:
                rval = s.translate_to_c()
                if len(rval) and rval[-1]!="\n":
                    rval += [";"]
                self.__c_tokens.extend(rval)
            if self.comment:
                self.__c_tokens += [" //"+self.comment+" \n"]  # trailing space so no antislash could spoil it
            else:
                 self.__c_tokens += ["\n"]
            for i in range(0,self.nesting):
                self.__c_tokens += ["}\n"]


        def _warn(self,line,msg):
            self.parent._warn(line,msg)
        def _info(self,line,msg):
            self.parent._info(line,msg)

        def translate_to_c_second_pass(self):
            if self.parent.generation_options.clean_code:
                if self.goto and self.case_statement:
                    self._warn(self.line,"goto & case statement on same line")
                if self.gosub and self.case_statement:
                    self._warn(self.line,"gosub & case statement on same line, will ignore case statement")
                    self.case_statement = False
                if self.goto and self.gosub:
                    self._warn(self.line,"goto & gosub on same line, ignoring GOTO")
                    self.goto = False

            rval = ""
            label_insertion = self.goto or (not self.parent.generation_options.clean_code and self.gosub)
            if label_insertion:
                rval = "\nlabel_%d:\n" % self.line

            elif self.gosub:
                # clean code
                rval = "\n}\n\nvoid BP_%05d()\n{\n" % self.line
            if self.case_statement:
                rval = "\ncase %d:\n" % self.line
            if rval != "":
                self.__c_tokens.insert(0,rval)
                # avoid label and "}" just after
                if len(self.__c_tokens)>2 and self.__c_tokens[2].strip() == "}":
                    self.__c_tokens.insert(1,";")



            return self.__c_tokens

        def add_statement(self,statement):
            statement.compute_tree()

            statement.fix_tree()

            if len(statement.tokens)>0:
                t = statement.tokens[0]
                if t.type == t.KEYWORD and t.value=="NEXT":
                    # check if previous statement is not a FOR loop
                    # that would be a deliberate active loop: in that case,
                    # insert a delay to simulate that
                    if len(self.statements)>0:
                        ps = self.statements[-1]
                        if len(ps.tokens)>0:
                            pt = ps.tokens[0]
                            if pt.type == t.KEYWORD and pt.value == "FOR":
                                delay_statement = self.parent.BasicStatement(self.parent)
                                wt = self.parent.Token(self.parent,"_LOOP_WAIT",t.KEYWORD)
                                self._info(t.line,"inserted temporisation in empty FOR loop, adjust if needed")
                                delay_statement.tokens.append(wt)
                                self.add_statement(delay_statement)

            self.statements.append(statement)


    @staticmethod
    def __until_non_char(raw_line,line_pos,c):
        rval = -1
        for i in range(line_pos,len(raw_line)):
            if raw_line[i]!=c:
                rval = i
                break
        return rval
    @staticmethod
    def __until_non_space(raw_line,line_pos):
        return Bas2C.__until_non_char(raw_line,line_pos,' ')

    def __syntax_error(self,message):
        line_pos = self._line_pos + len(str(self._lineno))+2 # adjust to take line number into account
        self.__error("%d:%d: %s" % (self._lineno,line_pos,message))

    def __process_line(self):
        raw_line = self.__raw_lines_dict[self._lineno]
        rval = self.BasicLine(self,self._lineno,raw_line)
        self._line_pos = 0
        self._nesting = 0

        statement = self.BasicStatement(self)

        if_flag = False

        while True:
            is_kw = False
            # first test keywords
            self._line_pos = self.__until_non_space(raw_line,self._line_pos)
            if self._line_pos==-1:
                # end
                rval.add_statement(statement)
                break
            for kw in self.__KEYWORDS_KEYS:
                kw_pos = raw_line.find(kw,self._line_pos) # no regex here, faster
                if kw_pos == self._line_pos:
                    # basic keyword found
                    t = self.Token(self,kw,ttype=self.Token.KEYWORD)
                    self._line_pos += len(kw)
                    is_kw = True
                    break
            if is_kw:
                if kw in ["REM","'"]:
                    if len(statement.tokens):
                        rval.add_statement(statement)
                        statement = self.BasicStatement(self)
                    rval.comment = raw_line[self._line_pos:]
                    break  # end of processing
                elif kw == "DATA":
                    data_line = raw_line[self._line_pos:]

                    rval.comment = "$DATA$"+data_line
                    self.__add_data_line(data_line)
                    break  # end of processing
                elif kw==":":
                    # next statement
                    if self._nesting!=0:
                        self._info(self._lineno,"pos:%d: opening parenthesis mismatch (%d)" % (self._line_pos,self._nesting))
                        self._nesting = 0
                    rval.add_statement(statement)
                    statement = self.BasicStatement(self)

                else:
                    if kw=="IF":
                        if_flag=True
                    elif kw=="GOTO":
                        if if_flag:
                            # change IF cond GOTO => IF cond THEN
                            t.value="THEN"
                            if_flag = False
                    elif kw=="THEN":
                        if_flag=False
                    elif kw=="(":
                        # increase nesting from next token
                        self._nesting += 1
                        # if previous token is a function or
                        # an array variable, increase nesting right now
                        if len(statement.tokens)==0:
                            pass
                        else:
                            prev_token = statement.tokens[-1]
                            if prev_token.function or prev_token.type==self.Token.ARRAY_VARIABLE:
                                t.nest()

                    elif kw==")":
                        if self._nesting > 0:
                            self._nesting -= 1
                        else:
                            self._info(self._lineno,"pos:%d: closing parenthesis mismatch (fixed)" % (self._line_pos))
                    # store token
                    statement.tokens.append(t)
            else:
                if len(statement.tokens)>0:
                    previous_token = statement.tokens[-1]
                    if previous_token.function:
                        # function but not followed by open parenthesis: ex PRINT SPC 12): artificially add one
                        # increase nesting from next token
                        self._nesting += 1
                        parent_t = previous_token.clone()
                        parent_t.value = "("
                        parent_t.nest()
                        statement.tokens.append(parent_t)
                        self._info(self._lineno,"pos:%d: added missing parenthesis after function %s" % (self._line_pos,previous_token.value))
                # test variables
                m = self.__VARIABLE_RE.match(raw_line,self._line_pos)
                t = None
                if m != None:
                    value = m.group(1)
                    next_pos = m.end()
                    nsp = self.__until_non_space(raw_line,next_pos)
                    if nsp!=-1 and raw_line[nsp]=='(':
                        # must be an array variable, we don't know the size unless has been added
                        # by DIM, but can happen
                        self.__unbounded_array_variables.add(value)
                        t = self.Token(self,value,self.Token.ARRAY_VARIABLE)
                    else:
                        # normal variable
                        self.__scalar_variables.add(value)
                        t = self.Token(self,value,self.Token.SCALAR_VARIABLE)
                else:
                    m = self.__FLOAT_RE.match(raw_line,self._line_pos)
                    if m != None:
                        value = m.group(1)
                        t = self.Token(self,float(value),self.Token.FLOAT)
                    else:
                        m = self.__INTEGER_RE.match(raw_line,self._line_pos)
                        if m != None:
                            value = m.group(1)
                            if value[0]=='#':
                                t = self.Token(self,int(value[1:],16),self.Token.HEX_INTEGER)
                            else:
                                t = self.Token(self,int(value),self.Token.INTEGER)

                        else:
                            m = self.__STRING_RE.match(raw_line,self._line_pos)
                            if m == None:
                                m = self.__BAD_STRING_RE.match(raw_line,self._line_pos)
                            if m != None:
                                value = m.group(1)
                                t = self.Token(self,value,self.Token.STRING)
                            else:
                                self.__syntax_error("unknown token")
                if m != None:
                    statement.tokens.append(t)
                    self._line_pos = m.end()

        return rval

    def get_token_repr(self,t):
        if t.type == t.STRING and self.generation_options.localize_strings:
            v = str(t)
            # lookup v in the strings table
            if v in self.__value2constid:
                # already there
                constid = self.__value2constid[v]
            else:
                # generate a new id
                self.__constid_index += 1
                constid = "SC_%03d" % self.__constid_index
                self.__constid2value[constid] = v
                self.__value2constid[v] = constid
            string_value = GLOBAL_CONSTANT_POINTER+"->"+constid + " /* "+v+" */ "

        else:
            # all other cases: direct string rep
            string_value = str(t)
        if t.type in [t.SCALAR_VARIABLE,t.ARRAY_VARIABLE]:
            if string_value[-1]=="$":
                string_value = "str_"+string_value[:-1]
            elif string_value[-1]=="%":
                string_value = "int_"+string_value[:-1]
            if t.type == t.ARRAY_VARIABLE:
                    string_value = GLOBAL_ARRAY_STRUCT+"."+string_value
            else:
                string_value = GLOBAL_SCALAR_STRUCT+"."+string_value

        return string_value

    def mutate_to_int(self,token):
        if self.generation_options.mutate_floats:
            self._info(token.line,"Mutated float %s to integer type cos involved in logical operation" % (token.value))
            if token.type == token.ARRAY_VARIABLE:
                self.__forced_int_array.add(token.value)
            elif token.type == token.SCALAR_VARIABLE:
                self.__forced_int_scalar.add(token.value)
        else:
            self._warn(token.line,"Should manually mutate float %s to integer type cos involved in logical operation or use --mutate option" % (token.value))

    def set_scalar_input(self,value):
        self.__scalar_input = value
    def add_procedure(self,procedure_name):
        self.__procedures.add(procedure_name)
    def add_static_asm_call(self,address):
        self.__static_asm_call.add(address)
    def add_array_variable(self,variable_name,size_expression):
        self.__array_variables[variable_name] = size_expression
    def __check_variable_names(self,s,vt):
        vd=dict()
        for i in s:
            ei = i[-1]
            if (vt==self.Token.INTEGER and ei=="%") or (vt==self.Token.FLOAT and ei.isalnum()) or (vt==self.Token.STRING and ei=='$'):
                if len(i)>1:
                    i2 = i[0:2]
                    if i2 not in vd:
                        vd[i2] = []
                    vd[i2].append(i)
        for k,v in vd.items():
            if len(v)>1:
                self._warn(-1,"Variable %s is represented by several names: %s" % (k,",".join(v)))


    def __doit(self):
        # main processing here
        python_lacks.create_directory(self.__output_directory)

        self.__logfile = os.path.join(self.__output_directory,"warnings.txt")
        self.__purge_log()
        lines = python_lacks.read_file(self.__input_file)

        self.__raw_lines_dict = dict()
        self.__line_number = list()

        # build dictionary: line number => raw line
        for l in lines:
            m= self.__LINENUM_RE.match(l)
            if m != None:
                line_number = int(m.group(1))
                rest = l[m.end()+1:]
                self.__raw_lines_dict[line_number] = rest
                self.__line_number.append(line_number)

        self.__KEYWORDS_KEYS = sorted(self.BasicStatement._KEYWORDS_DICT,key = lambda entry: len(entry),reverse=True)

        self.lines_dict = dict()
        previous_rval = None
        for lineno in self.__line_number:
            self._lineno = lineno
            rval = self.__process_line()
            if lineno in self.lines_dict:
                self.__error("Duplicate line %d in source" % lineno)
            if len(rval.statements) and previous_rval != None and previous_rval.has_sequence_break():
                if self.generation_options.clean_code:
                    self._info(lineno,"Inserted label because previous line rendered code statically unreachable")
                    rval.goto = True
                else:
                    self._info(lineno,"Inserted GOSUB table entry because previous line rendered code statically unreachable")
                    rval.gosub = True
            self.lines_dict[lineno] = rval
            previous_rval = rval

        # insert manual "case"/label statements
        for cs in self.__case_statement:
            cl = cs.split(",")
            if len(cl)!=3:
                self.__error("Case statement syntax error: %s" % cs)
            start = int(cl[0])
            end = int(cl[1])
            step = int(cl[2])
            for lineno in range(start,end+step,step):
                if lineno in self.lines_dict:
                    if self.generation_options.clean_code:
                        self.lines_dict[lineno].case_statement = True
                    else:
                        self.lines_dict[lineno].gosub = True

        for lineno in self.__line_number:
            self._lineno = lineno
            self.lines_dict[lineno].translate_to_c_first_pass()
        # header
        rval = []
        rval.append('#include "oric_basic.h"\n')
        rval.append('#include <stdio.h>\n')
        rval.append('#include <stdlib.h>\n')

        if self.generation_options.floats_disabled:
            rval.append("#define T_NUMERIC T_INT\n")

        if len(self.__data_array):
            rval.extend(["static const T_STRING_PTR ",DATA_ARRAY,"[","]","=","\n{\n"])
            first_data=True
            nb_data = 0
            for d in self.__data_array:
                if first_data:
                    first_data=False
                else:
                    rval.append(",")
                rval.append('"%s"' % d)
                if nb_data==10:
                    rval.append("\n")
                    nb_data = 0
                nb_data+=1

            rval.append("\n}")
            rval.append(";\n")
            # we know the size which is len(self.__data_array) but this method allows to modify the number of data in C code afterwards
            rval.extend(["static const int total_nb_data","=","sizeof(",DATA_ARRAY,")/sizeof(T_STRING_PTR)",";\n\n"])

        tds = ["\n","typedef"," ","struct","\n","{","\n"]
        if len(self.__scalar_variables):
            if self.__scalar_input:
                rval.append("\n\nstatic T_STRING %s;" % TEMP_STRING_VAR)
            rval.extend(tds)
            for k in sorted(self.__scalar_variables):
                if k[-1]=="%":
                    t = "T_INT"
                    v = k[:-1]
                    prefix = "int_"
                elif k[-1]=="$":
                    t = "T_STRING"
                    v = k[:-1]
                    prefix = "str_"
                else:
                    if k in self.__forced_int_scalar:
                        t = "T_INT"
                    else:
                        t = "T_NUMERIC"
                    prefix = ""
                    v = k
                rval.extend([t," ",prefix+v,";","\n"])
            rval.extend(["\n","}"," ","ScalarGlobals;\n\nstatic ScalarGlobals %s;\n" % GLOBAL_SCALAR_STRUCT,"\n"])

        # detect the array variables that have not been declared through dim

        self.__unbounded_array_variables.difference_update(set(self.__array_variables.keys()))
        for i in sorted(self.__unbounded_array_variables):
            self._warn(-1,"implicity declared array variable %s. Assuming size to %d, please adjust if needed" % (i,self._guessed_array_size))
            self.__array_variables[i] = [str(self._guessed_array_size)]

        if self.__array_variables:
            array_tokens = list(tds)

            for k,sz in sorted(self.__array_variables.items()):
                if k[-1]=="%":
                    t = "T_INT"
                    prefix = "int_"
                    v = k[:-1]
                elif k[-1]=="$":
                    t = "T_STRING"
                    prefix = "str_"
                    v = k[:-1]
                else:
                    if k in self.__forced_int_scalar:
                        t = "T_INT"
                    else:
                        t = "T_NUMERIC"
                    prefix = ""
                    v = k
                array_tokens.extend([t," ",prefix+v,"["]+sz+["]",";","\n"])
            array_tokens.extend(["\n","}"," ","ArrayGlobals;\n\nstatic ArrayGlobals %s;\n\n"% (GLOBAL_ARRAY_STRUCT)])
            rval.extend(array_tokens);

        if self.__constid2value:
            # localization
            rval.append("\n\ntypedef struct\n")
            rval.append("{")
            ks = sorted(self.__constid2value.keys())
            for k in ks:
                rval.extend(["\n","const"," ","T_STRING_PTR"," ",k,";"])
            rval.extend(["\n","}"," ","StringConstants",";","\n\n"])

            rval.extend(["static"," ","StringConstants"," ","sc","=","\n","{"])
            first_val = True
            for k in ks:
                v = self.__constid2value[k]
                if first_val:
                    first_val = False
                else:
                    rval.append(",")
                rval.extend(["\n",v])
            rval.extend(["\n","}",";\n","\n"])

            rval.extend(["static"," ","const"," ","StringConstants"," ","*",GLOBAL_CONSTANT_POINTER,"=","&sc",";","\n\n"])

        write_scalars = ""
        write_arrays = ""
        read_scalars = ""
        read_arrays = ""
        if len(self.__scalar_variables):
            read_scalars = "fread(&gs,sizeof(gs),1,f);\n"
            write_scalars = "fwrite(&gs,sizeof(gs),1,f);\n"
        if len(self.__array_variables):
            read_arrays = "fread(&gv,sizeof(gv),1,f);\n"
            write_arrays = "fwrite(&gv,sizeof(gv),1,f);\n"

        rval.append("""
#if 0  /* remove if needed */
static const char *gsn = "gamesave";
static T_INT load_game()
{
    FILE *f = fopen(gsn,"rb");
    if (f != 0)
    {
       %s%s   fclose(f);
    }
    return (f != 0);
}
static T_INT save_game()
{
    FILE *f = fopen(gsn,"wb");
    if (f != 0)
    {
	   %s%s	   fclose(f);
    }
    return (f != 0);

}
#endif
""" % (read_scalars,read_arrays,write_scalars,write_arrays))


        procs = sorted(self.__procedures)

        for p in procs:
            if p[0]=="B":
                rval.extend(["static"," "])  # BASIC procedures are local
            rval.extend(["void"," "])
            rval.append(p)
            rval.extend(["(",")",";","\n"])

        rval.append("\n")

        # clear is implemented as a macro
        rval.extend(["#define"," ","CLEAR","(",")"," "])
        if len(self.__scalar_variables):
            rval.extend(["memset(&%s,0,sizeof(%s))" % (GLOBAL_SCALAR_STRUCT,GLOBAL_SCALAR_STRUCT)])
        if len(self.__array_variables):
            rval.extend([";","memset(&%s,0,sizeof(%s))" % (GLOBAL_ARRAY_STRUCT,GLOBAL_ARRAY_STRUCT)])
        rval.append("\n")
        rval.append("\n")

        if self.generation_options.clean_code:
            goto_handler_toks = ["static void goto_handler(T_INT line_number, T_INT basic_caller_line)","\n","{","\n","switch(line_number)","\n","{","\n",
                "default:","\n",r'error("GOTO: unsupported line number ",str$(line_number)," called from ",str$(basic_caller_line),0);',
                "\n","}","\n","}","\n"]
            if self._goto_handler_needed:
                rval.extend(goto_handler_toks)
            if self._gosub_handler_needed:
                    # insert dynamic gosub handler
                    for i in goto_handler_toks:
                        rval.append(i.replace("goto","gosub").replace("GOTO","GOSUB"))


        if not self.generation_options.clean_code:
            # just create the GOSUB & POP macros & gosub_handler definition: emulate POP through a stack counter
            rval.extend(["static"," ","int"," ","current_stack","=","0",";","\n","\n"])
            rval.extend(["#define"," ","GOTO","(","linenum",")"," ","line_number","=","linenum",";","goto"," ",
            "line_switch","\n"])
            rval.extend(["#define"," ","GOSUB","(","linenum",")"," ","{","int"," ","sstk","=","current_stack",";","current_stack++",";","\\","\n",
            "gosub_handler","(","linenum",",","basic_caller_line",")",";","\\","\n","current_stack--",";"," ","if"," ","(","current_stack","!=","sstk",")"," ",
            "{","current_stack","++",";","return",";","}","\\","\n","}","\n"])
            rval.extend(["#define"," ","POP","(",")"," ","current_stack","--",";","if"," ","(","current_stack","<","0",")"," ","error","(",
            '"POP: out of stack"',",","NULL",")","\n","\n"])
            rval.extend(["static void gosub_handler(int line_number,int basic_caller_line);\n"])  # main handler

        if self._call_handler_needed:
            rval.extend(["extern void call_handler(T_INT address, T_INT basic_caller_line,int from_basic)",";","\n"]) # reference call handler from asm2c file
            rval.extend(["extern void mem_init()",";","\n","\n"])  # init oric memory with stuff from bin2c .h file

        rval.extend(["void basic_entrypoint()\n","{","\n","CLEAR();","\n"])
        if self._call_handler_needed:
            rval.extend(["mem_init","(",")",";","\n"])
        # init basic afterwards since it sets some zero page values
        rval.extend(["basic_init","(",")",";","\n"])

        if self.generation_options.clean_code:
            pass
        else:

            # entrypoint just calls the gosub_handler on the first line
            self.lines_dict[self.__line_number[0]].gosub = True
            rval.extend(["gosub_handler","(",str(self.__line_number[0]),",","-1",")",";","\n","}","\n","\n"])
            rval.extend(["void gosub_handler(int line_number,int basic_caller_line)\n","{","\n","line_switch:","\n","switch(line_number)","\n","{"])
            for lineno in self.__line_number:
                if self.lines_dict[lineno].gosub:
                    rval.extend(["\n","case"," ",str(lineno),":"," ","goto"," ","label_%d" % lineno,";"])
            rval.extend(["\n","default:","\n",r'error("GOSUB: unsupported line number ",str$(line_number)," called from ",str$(basic_caller_line),0);',
            "\n","}","\n","\n"])

        for lineno in self.__line_number:
            self._lineno = lineno # useless?
            #last_tokens_return = rval[-2]=="return"

            if self.generation_options.clean_code and self.lines_dict[lineno].gosub:
                return_token = rval[-3]
                if return_token!="return":
                    self._warn(lineno,"gosub target not preceded by RETURN (found '%s')" % (return_token))
            rval.extend(["/* %d */ " % lineno])
            rval.extend(self.lines_dict[lineno].translate_to_c_second_pass())

        # warn about variables with same 2 starting letters but different name
        for t in [self.Token.FLOAT,self.Token.INTEGER,self.Token.STRING]:
            self.__check_variable_names(self.__scalar_variables,t)
            self.__check_variable_names(self.__array_variables,t)

        # cosmetic C post-processing:
        # fix useless double "((" "))": ((A+B)) => (A+B). (some will remain if triple ((( or ((A+(A+2)+C))
        # fix useless double ;;
        # fix "}" followed by ";" (the "};" construction exists but not in our generator)
        removal_positions = []
        nb_toks = len(rval)
        for i in range(1,nb_toks):
            if i>0:
                simplify=False
                if rval[i]=='(' and rval[i-1]=='(':
                    # find the closing parenthesis
                    simplify = False
                    for j in range(+1,nb_toks-1):
                        if rval[j]==')':
                            if rval[j+1]==')':
                                simplify = True
                            break
                    if simplify:
                        removal_positions.append(i)
                        removal_positions.append(j)
                elif rval[i]==';' and rval[i-1]==';':
                    removal_positions.append(i)
                elif rval[i]==';' and rval[i-1].strip() in ['}','{']:
                    removal_positions.append(i)

        # remove indexes by the end to avoid shifting
        removal_positions.sort(reverse=True)
        for rp in removal_positions:
            rval.pop(rp)

        removal_positions = []
        # pass 2: reduce a = a + 1 => a+=1
        nb_toks = len(rval)
        for i in range(0,nb_toks-4):
            t1 = rval[i+1].strip()
            t3 = rval[i+3].strip()
            if t1=='=' and rval[i]==rval[i+2] and t3 in ["*","/","+","-"]:
                if rval[i+4]=="1" and t3 in ["+","-"]:
                    rval[i+1] = t3+t3
                    removal_positions.append(i+4)
                else:
                    rval[i+1]=" "+t3+"= "
                removal_positions.append(i+2)
                removal_positions.append(i+3)

        # remove indexes by the end to avoid shifting
        removal_positions.sort(reverse=True)
        for rp in removal_positions:
            rval.pop(rp)

        rval.append("\n}\n")

        output_handle = open(os.path.join(self.__output_directory,"basic.c"),"w")
        output_handle.write("/* generated by JOTD's bas2c.py version %s */\n\n" % self.__VERSION_NUMBER)
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

        if len(self.__static_asm_call):
            asm_call_file = os.path.join(self.__output_directory,"asm_entrypoints.txt")
            output_handle = open(asm_call_file,"w")
            for t in sorted(self.__static_asm_call):
                output_handle.write("0x%x\n" % t)
            output_handle.close()




if __name__ == '__main__':
    """
        Description :
            Main application body
    """


    o = Bas2C()
    o._init_from_sys_args(debug_mode = True)
    #o.init("output_file")
