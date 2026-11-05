import os,re,fileinput,sys,shutil
import threading

class Tee(object):
    STDOUT_TYPE = 1
    STDERR_TYPE = 2

    MAX_CHAR_PER_PRINT = 49125

    def __init__(self, logfile=None, display=True):
        if logfile:
            self.logfile = self.file = open(logfile, "wb")
        else:
            self.logfile = None

        self.__display = display

        self.lock = threading.RLock()

        self.stdout = sys.stdout
        self.stderr = sys.stderr
        sys.stdout = TeeChild(self, Tee.STDOUT_TYPE)
        sys.stderr = TeeChild(self, Tee.STDERR_TYPE)

    def __del__(self):
        self.lock.acquire()

        sys.stdout = self.stdout
        sys.stdout = self.stderr

        if self.logfile:
            self.logfile.close()

        self.lock.release()

    def write(self, type, data):
        self.lock.acquire()

        if self.logfile:
            self.logfile.write(data)

        if self.__display:
            while len(data) > 0:
                if type == Tee.STDOUT_TYPE:
                    self.stdout.write(data[:self.MAX_CHAR_PER_PRINT])
                elif type == Tee.STDERR_TYPE:
                    self.stderr.write(data[:self.MAX_CHAR_PER_PRINT])

                data = data[self.MAX_CHAR_PER_PRINT:]

        # Try to keep same order between print display and logfile
        self.flush(type)

        self.lock.release()

    def flush(self, type):
        self.lock.acquire()

        if self.logfile:
            self.logfile.flush()

        if self.__display:
            if type == Tee.STDOUT_TYPE:
                try:
                    self.stdout.flush()
                except IOError: # Error occurs when using ssh_it.py
                    pass
            elif type == Tee.STDERR_TYPE:
                try:
                    self.stderr.flush()
                except IOError: # Error occurs when using ssh_it.py
                    pass

        self.lock.release()


class TeeChild(object):
    def __init__(self, tee, type):
        self.tee = tee
        self.type = type

    def write(self, data):
        self.tee.write(self.type, data)

    def flush(self):
        self.tee.flush(self.type)

## {{{ http://code.activestate.com/recipes/252524/ (r3)
class Node(object):
    __slots__ = ['prev', 'next', 'me']
    def __init__(self, prev, me):
        self.prev = prev
        self.me = me
        self.next = None

class LRU:
    """
    Implementation of a length-limited O(1) LRU queue.
    Built for and used by PyPE:
    http://pype.sourceforge.net
    Copyright 2003 Josiah Carlson.
    """
    def __init__(self, count, pairs=[]):
        self.count = max(count, 1)
        self.d = {}
        self.first = None
        self.last = None
        for key, value in pairs:
            self[key] = value
    def __contains__(self, obj):
        return obj in self.d
    def __getitem__(self, obj):
        a = self.d[obj].me
        self[a[0]] = a[1]
        return a[1]
    def __setitem__(self, obj, val):
        if obj in self.d:
            del self[obj]
        nobj = Node(self.last, (obj, val))
        if self.first is None:
            self.first = nobj
        if self.last:
            self.last.next = nobj
        self.last = nobj
        self.d[obj] = nobj
        if len(self.d) > self.count:
            if self.first == self.last:
                self.first = None
                self.last = None
                return
            a = self.first
            a.next.prev = None
            self.first = a.next
            a.next = None
            del self.d[a.me[0]]
            del a
    def __delitem__(self, obj):
        nobj = self.d[obj]
        if nobj.prev:
            nobj.prev.next = nobj.next
        else:
            self.first = nobj.next
        if nobj.next:
            nobj.next.prev = nobj.prev
        else:
            self.last = nobj.prev
        del self.d[obj]
    def __iter__(self):
        cur = self.first
        while cur != None:
            cur2 = cur.next
            yield cur.me[1]
            cur = cur2
    def iteritems(self):
        cur = self.first
        while cur != None:
            cur2 = cur.next
            yield cur.me
            cur = cur2
    def iterkeys(self):
        return iter(self.d)
    def itervalues(self):
        for i,j in self.iteritems():
            yield j
    def has_key(self,k):
        return self.d.has_key(k)
    def keys(self):
        return self.d.keys()
## end of http://code.activestate.com/recipes/252524/ }}}


# use with ConfigFile object
def ConfigSectionMap(config,section):
    dict1 = {}
    options = config.options(section)
    for option in options:
        try:
            dict1[option] = config.get(section, option)
            if dict1[option] == -1:
                print("skip: %s" % option)
        except:
            raise Exception("exception on %s!" % option)
            dict1[option] = None
    return dict1


## Daemon class
import time

class Daemon(threading.Thread):
    def __init__(self, target, period):
        threading.Thread.__init__(self)
        self.__target = target
        self.__period = period

    def setPeriod(self, period):
        self.__period = period

    def start(self):
        # recreate the thread object to allow a restart call
        if self.isAlive():
            self.stop()
            self.join()
        threading.Thread.__init__(self)
        threading.Thread.setDaemon(self, True)
        threading.Thread.start(self)

    def run(self):
        # init/change the variables
        self.__destroy = False

        sleep_slice = 0.1
        # init timer to period + 1 to launch the tool during the first loop
        timer = self.__period+1
        while True:
            if self.__destroy:
                break

            # execute the target every period
            if timer > self.__period:
                timer = 0
                self.__target()
            timer += sleep_slice
            time.sleep(sleep_slice)

    def stop(self):
        # set the destroy flag to True. It will finish the infinite execution loop
        self.__destroy = True
## end of Daemon class

def if_else(condition,result_for_true,result_for_false):
    """
        equivalent of ternary operator in C, Java... (  condition ? result_for_true : result_for_false )

        ex:
        >>> if_else(True,True,False)
        True
        >>> if_else(False,True,False)
        False
    """
    if(condition):
        return result_for_true
    else:
        return result_for_false

def normpath_list(l):
    """
    normalize a list of file paths
    """
    rval = []
    for i in l:
        rval.append(os.path.normpath(i))
    return rval

def flatten(l):
    """
        take a list with nested sublist and make it flat
        ex:

        >>> flatten([[[1],2,[3,4]],5,[6],[7,8,9]])
        [1, 2, 3, 4, 5, 6, 7, 8, 9]
    """
    result = []
    for x in l:
        if isinstance(x,(list,tuple,set)):
            result.extend(flatten(x))
        else:
            result.append(x)
    return result

def hex2dec(hex_str):
    """
        return decimal value (as int) from hexadecimal value passed in a string

        ex:
        >>> hex2dec('0x10')
        16
        >>> hex2dec('10')
        16
        >>> hex2dec('0x00')
        0
    """
    return int(hex_str,16)

def uniq_list(the_list):
    """
    take a list and returns a new one without duplicates,
    but preserves the order of the list (as opposed to uniq below
    """

    rval = []
    previous_value=None
    for l in the_list:
        if previous_value != l:
            rval.append(l)
        previous_value = l
    return rval

def uniq(array):
    """
        take a list and return a new one without double value

        ex:
        >>> uniq([1,1,1,2,2,3,3,4,4,4,4,4,4,5,6,7,7,7,7,7,8,9])
        [1, 2, 3, 4, 5, 6, 7, 8, 9]
        >>> uniq((1,1,1,2,2,3,3,4,4,4,4,4,4,5,6,7,7,7,7,7,8,9))
        [1, 2, 3, 4, 5, 6, 7, 8, 9]
        >>> uniq(['a','z','a','b','a','z','b'])
        ['a', 'z', 'b']
    """
    return list(set(array))

def __line__():
    """
        return the number of the current executing line

        ex:
        for i in range(0,5):
            print __line__()
        2
        2
        2
        2
        2
    """
    import inspect
    return inspect.currentframe().f_back.f_lineno

def number_suffix(nb):
    """
    return st for 1, nd for 2, rd for 3, th for others
    """
    if nb>0 and nb<4:
        rval = ["st","nd","rd"][nb-1]
    else:
        rval = "th"
    return rval

def search_in_file(filename,pattern):
    """
        search the first line which match the given pattern (simple string comparison)
        and return its number (start from 1) or -1 if no line match

        ex:
        >>> search_in_file(__file__,"def search_in_file(filename,pattern):")==search_in_file.func_code.co_firstlineno
        True
        >>> search_in_file(__file__,"Tout le monde s'eclate chez" + "THALES")
        -1
    """
    try:
        found = False
        line_no = 1
        f = open(filename,'r')
        for line in f:
            if line.find(pattern) != -1:
                found = True
                break
            line_no += 1
        f.close()
        if found:
            return line_no
        else:
            return -1
    except:
        import traceback
        traceback.print_exc
        return -1


def __sub_env_function(g):
    env_variable = g.group(1)[1:] # not the "$"
    if env_variable.endswith("%"):
        env_variable = env_variable[:-1] # not the last "%"
    value = g.group(1)
    if os.environ.has_key(env_variable):
        value = os.environ[env_variable]
    else:
        # special cases (unix style)
        if env_variable in ["PWD","CD"]:
            value = os.getcwd()

    return value

def timeout_run(func, args=(), kwargs={}, timeout=10, default=None):
    """This function will spawn a thread and run the given function
    using the args, kwargs and return the given default value if the
    timeout is exceeded.
    """
    class InterruptableThread(threading.Thread):
        def __init__(self):
            threading.Thread.__init__(self)
            self.result = default
        def run(self):
            self.result = func(*args, **kwargs)
    it = InterruptableThread()
    it.start()
    it.join(timeout)
    rval = it.result
    return rval

def evaluate_environment_variables(s):
    """
    replaces occurrences of env. variables by their values:
    evaluate_environment_variables("$HOME/foo") => "/users/me/foo"
    """

    rval = s
    while True:
        new_rval = re.sub(r"(\$[A-Za-z0-9_]+)",__sub_env_function,rval)
        exit_loop = new_rval == rval

        rval = new_rval
        if exit_loop:
            # no more replacements to perform
            break
    while True:
        new_rval = re.sub(r"(%[A-Za-z0-9_]+%)",__sub_env_function,rval)
        exit_loop = new_rval == rval

        rval = new_rval
        if exit_loop:
            # no more replacements to perform
            break

    return os.path.normpath(rval)

def get_file_size(filepath):
    return os.stat(filepath)[6]

def compare_files(version_1,version_2):
    """
    compare files in binary mode
    """
    rval = False

    size1 = get_file_size(version_1)
    size2 = get_file_size(version_2)

    if size1 == size2:
        # same size: check contents
        fd1 = open(version_1)
        fc1 = fd1.read()
        fd1.close()
        fd2 = open(version_2)
        fc2 = fd2.read()
        fd2.close()
        # compare strings
        rval = (fc1 == fc2)

    return rval

def commonsuffix(ll):
    """
    same as os.path.commonprefix but for suffixes
    in: ll a list of strings
    out: the biggest common part of the end of all strings
    """

    rval = []
    # compute min size
    minsz = 1<<32
    for l in ll:
        ls = len(l)
        if ls < minsz:
            minsz = ls

    if minsz > 0:
        for i in xrange(1,minsz+1):
            isc=True
            cl=ll[0][-i]

            for l in ll:
                if l[-i] != cl:
                    isc = False
                    break
            if isc:
                rval.append(cl)
            else:
                break
    rval.reverse()
    s = ""
    for c in rval:
        s += c
    return s

def commonsuffix_digits(ll):
    """
    same as commonsuffix but for suffixes with digits (we don't consider differences with digits)
    in: ll a list of strings
    out: the biggest common part of the end of all strings as regex
    if digits are different, differences are ignored (replaced by \d regex)
    """

    rval = []
    # compute min size
    minsz = 1<<32
    for l in ll:
        ls = len(l)
        if ls < minsz:
            minsz = ls

    if minsz > 0:
        for i in xrange(1,minsz+1):
            isc=True
            cl=ll[0][-i]
            d = cl.isdigit()
            if d:
                cl = r"\d"
            for l in ll:
                if l[-i].isdigit() and d:
                    pass
                elif l[-i] != cl:
                    isc = False
                    break
            if isc:
                rval.append(cl)
            else:
                break
    rval.reverse()
    s = ""
    for c in rval:
        s += c
    return s

def dirname(p):
    """
    same as os.path.dirname except if that function returns "" in
    which case dirname will return "." instead
    """
    rval = os.path.dirname(p)
    if rval == "" and p != "":
        rval = "."
    return rval

def remove(p):
    """
    remove a file, removing possible write protection first
    """
    os.chmod(p,0o0777)
    os.remove(p)

def rmtree(dirpath):
    """
    really remove a directory tree
    (shutil.rmtree does not work properly on windows
    when read-only flags are set)
    does not crash with exception when directory does not exist
    """
    import xargs
    [rc,output] = xargs.execute("cmd /c rmdir /Q /S",[dirpath], startupinfo=xargs.startup_info_no_window())
    return [rc,output]

def rmcontent(dirpath):
    for root, dirs, files in os.walk(dirpath):
        for f in files:
            remove(os.path.join(root, f))
        for d in dirs:
        	rmtree(os.path.join(root, d))

def copytree(src, dst, symlinks=False, ignore=None):
    """Recursively copy a directory tree using copy2().

    The destination directory COULD exist.
    If exception(s) occur, an Error is raised with a list of reasons.

    If the optional symlinks flag is true, symbolic links in the
    source tree result in symbolic links in the destination tree; if
    it is false, the contents of the files pointed to by symbolic
    links are copied.

    XXX Consider this example code rather than the ultimate tool.

    """
    for item in os.listdir(src):
        s = os.path.join(src, item)
        d = os.path.join(dst, item)
        if os.path.isdir(s):
            shutil.copytree(s, d, symlinks, ignore)
        else:
            shutil.copyfile(s, d)

def blocked_file_write(filepath,buffer,mode="wb",block_size=10000000):
    """
    write a file using blocks to avoid the dreaded "invalid argument"
    when dumping big files to disk
    """
    f = open(filepath,mode)
    len_to_write = len(buffer)
    offset = 0
    while len_to_write > 0:
        l = min(block_size,len_to_write)
        f.write(buffer[offset:offset+l])
        offset += block_size
        len_to_write -= block_size
    f.close()

def read_file(filepath,remove_blank_lines=False):
    """
    read a file and returns a list of lines
    strips "\r"
    remove blank lines if requested
    """
    rval = []
    f = open(filepath,"r")
    for l in f:
        l = l.replace("\r","").replace("\n","")
        if not (remove_blank_lines and l == ""):
            rval.append(l)
    return rval

def write_file(filepath,text_lines,mode="w"):
    """
    write the list of strings "text_lines" on disk
    Use "u" mode to update the beginning of a text file
    """
    if mode != "u":
        f = open(filepath,mode)
        f.write("\n".join(text_lines))
        f.write("\n")
        f.close()
    else:
        f = fileinput.input(filepath,inplace=1)
        text_lines.reverse()
        __line_pre_adder(f, "\n".join(text_lines))

def __line_pre_adder(f,line_to_prepend):
    for xline in f:
        if f.isfirstline():
            print(line_to_prepend.rstrip('\r\n') + '\n' + xline,)
        else:
            print(xline,)

def get_fixed_case_path(p):
    """
    correct the passed path for proper casing
    p: path of a valid file, with wrong casing (windows)
    scans all directories of the path and replaces the directory names by the ones
    read by a "listfile" (thus retrieving the actual casing stored on the disk)

    Ex : (r" \\xtlsusclc20\\Echange\\fbe\\ams08005671-00c saj trajpred.doc")
    => \\xtlsusclc20\\Echange\\fbe\\AMS08005671-00C SAJ TRAJPRED.doc
    """
    rval = ""
    normp = os.path.normpath(p)
    if normp in [os.curdir,os.pardir]:
        # no need to normalize this path (and would fail!)
        rval = normp
    elif p.find("~") != -1:
        # probably a 8.3 path: ignore
        rval = p
    elif p.startswith(os.sep+os.sep):
        # UNC path: ignore (not useful because clearcase can't be used directly in most cases)
        rval = p
    else:
        absp = os.path.abspath(p)
        dl = absp.split(os.sep)

        # drive letter always in upper case

        if dl[0].endswith(":"):
            dl[0] = dl[0].upper()


        for path_item in dl[1:]:
            if rval == "":
            	rval = dl[0] + os.sep
            else:
            	rval = rval + os.sep

            found = False

            if os.path.isdir(rval):

            	items = os.listdir(rval)

            	for dir_item in items:
                    if (dir_item == path_item) or (dir_item.lower() == path_item.lower()):
                        rval = rval + dir_item
                        found = True
                        break

        if not found:
        	# not found: use default
        	rval = rval + path_item

        # last check to avoid big problems...
        if rval.find(os.sep+os.sep+os.sep) != -1:
            rval = p
        else:
            if not os.path.isabs(p):
                # remove absolute prefix
                rval = rval[len(absp)-len(normp):]
    return rval

__AWK_RE = re.compile(r"\s+([^\s]+)")

def awkfields(l):
    """
    return n fields in an array (separator is space)
    """
    l = " "+l
    return __AWK_RE.findall(l)

def deregexify(p):
    """
    escape all special chars
    replace backslash first, or else could be replaced twice!!

    obsolete, replace by re.escape()
    """
    return re.escape(p)

    #for c in ['\\','*','.','?','(',')','[',']','^','$','+','|']:
    #    p = p.replace(c,"\\"+c)
    #return p

def is_ascii_compliant(s):
    """
    returns True if the string is ascii compliant
    """
    rval = True

    for c in s:
        oc=ord(c)
        if oc >= 128 or (oc < 32 and oc not in [9,10,13]):
            rval = False
            break

    return rval

def best_similar_string_position(s,l):
    """
    return the position of the string in list l
    which matches s best
    """
    rval = 0
    if len(l) == 0:
        raise Exception("empty list")

    common_prefix_len = 0
    for i in xrange(0,len(l)):
        cpl = os.path.commonprefix([l[i],s])
        if len(cpl) > common_prefix_len:
            common_prefix_len = len(cpl)
            rval = i
            if common_prefix_len==len(s):
                # optim: we can't do any better!
                break
    return rval

def ascii_compliant(s,best_ascii_approximation=False):
    """
    returns the same string, but with characters above 127 converted
    to acceptable ascii value
    best_ascii_approximation : True: replaces accentuated letters/other symbols
    by something ascii but readable. If False, replaces by space
    """

    if best_ascii_approximation:
        return replace_special_character(s)
    else:
        rval = ""
        for c in s:
            oc = ord(c)
            if oc < 128 and (oc > 31 or oc in [9,10,13]):
                rval += c
            else:
                rval += " "
        return rval

def replace_special_character(p):
    conversion_dictionary = {(224,226,195) : 'a',
                            (232,233,234) : 'e',
                            (244,246) : 'o',
                            (249,251) : 'u',
                            231 : 'c',
                            199 : 'C',
                            (238,239) : 'i',
                            150 : '-',
                            176 : ' deg',
                            178 : "**2",
                            177 : "+/-",
                            (145,146) : "'"}

    for i in range (0,len(p)):
        # convert character if his ascii code is superior to 127
        change = False
        op = ord(p[i])
        if op > 127:
            # search the character as a key of the dictionary of conversion
            for keys in conversion_dictionary:
                if isinstance(keys, tuple):
                    for key in keys:
                        if key == op:
                            p = p[:i] + conversion_dictionary[keys] + p[i+1:]
                            change = True
                else:
                    if keys == op:
                        p = p[:i] + conversion_dictionary[keys] + p[i+1:]
                        change = True

            if not change: # character don't exist as a key of the dictionary of conversion
                # replace with a blank
                p = p[:i] + " " + p[i+1:]
        elif op < 32 and op not in [9,10,13]:
            # control character: replace with blank
            p = p[:i] + " " + p[i+1:]

    return p

def comment_position(p):
    # init comment indice to a non-correct value
    ind_comment = -1

    # create a copy of the line
    temp_line = p

    # escape all quoted character
    if len(temp_line) >= 3:
        for i in range(0,len(temp_line)-2):
            if temp_line[i] == "'" and temp_line[i+2] == "'":
                # replace 'c' by XXX
                temp_line = temp_line[:i] + "XXX" + temp_line[i+3:]

    # escape all strings
    is_string = False
    for i in range (0,len(temp_line)):

        if temp_line[i] == '"':
            if is_string:
                is_string = False
                if i+1 < len(temp_line):
                    temp_line = temp_line[:i] + "X" + temp_line[i+1:]
                else:
                    temp_line = temp_line[:i] + "X"
            else:
                is_string = True

        if is_string:
            temp_line = temp_line[:i] + "X" + temp_line[i+1:]

    # find the indice of "--"
    i = 0
    while i < len(temp_line)+1 and ind_comment == -1:
        if temp_line[i:i+2] == "--":
            ind_comment = i
        i += 1
    return ind_comment

def terminate_process(pid,force=False,subprocesses=False):
    """
    try to terminate a process
    force: if not set, tries to terminate the process in a friendly way

    return True if process could be terminated
    """

    cmd = "taskkill "
    if force:
        # force, and terminate subprocesses
        cmd += "/F "
    if subprocesses:
        cmd += "/T "
    cmd += "/PID "+str(pid)
    import xargs
    [rc,output] = xargs.execute(cmd, startupinfo = xargs.startup_info_no_window())

    return rc==0

def list_basename(filelist):
    """
    return sorted basenames for the filelist
    """
    rval = []
    for l in filelist:
        rval.append(os.path.basename(l))
    rval.sort()
    return rval

def create_directory(dirpath):
    """
    create dir, but do not crash if already exists
    """

    if os.path.isdir(dirpath):
        pass
    else:
        try:
            os.makedirs(dirpath)
        except:
            # avoid exception for concurrent creation
            pass
        # we check directory existence in the end, so no sweat
        if not os.path.isdir(dirpath):
            raise Exception("Directory %s could not be created" % dirpath)

if __name__=='__main__':
    import doctest
    import inspect
    doctest.testmod()
