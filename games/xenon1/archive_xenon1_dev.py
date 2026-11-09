import os, make_archive_no_cc as make_archive,which



main_directory = "Xenon1"
lib_directory = "libasm6502"

m = make_archive.MakeArchive()

# generic part

srcgen = lib_directory

m.chdir(lib_directory)

for d in ["obj","lib"]:
    m.add_directory_entry(d)
for d in ["src","tools"]:
    m.add_directory(d,os.path.join(lib_directory,d))
m.add_file("libasm6502.gpr",lib_directory)
#

m.chdir(main_directory)

for d in ["src"]:
    m.add_directory(d,os.path.join(main_directory,d))

m.add_directory_entry("obj")

m.add_file("Xenon1.gpr",main_directory)


m.add_file("readme_coders.txt",main_directory)
m.create_zip("../Xenon1_dev.zip")
