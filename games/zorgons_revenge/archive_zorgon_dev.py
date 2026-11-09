import os, make_archive_no_cc as make_archive,which



main_directory = "ZorgonsRevenge"
lib_directory = "liboric"

m = make_archive.MakeArchive()

# generic part

srcgen = lib_directory

m.chdir(lib_directory)

for d in ["obj","lib"]:
    m.add_directory_entry(d)
for d in ["src","tools"]:
    m.add_directory(d,os.path.join(lib_directory,d))
m.add_file("liboric.gpr",lib_directory)
#

m.chdir(main_directory)

for d in ["src"]:
    m.add_directory(d,os.path.join(main_directory,d))

m.add_directory_entry("obj")

m.add_file("ZorgonsRevenge.gpr",main_directory)

for f in ["readme_coders.txt","build_release.bat","build.bat"]:
    m.add_file(f,main_directory)

m.create_zip("../ZorgonsRevenge_dev.zip")
