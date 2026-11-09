import os, make_archive_no_cc as make_archive,which

runenhanced="""
xenon1 -e
"""

main_directory = "Xenon1"

m = make_archive.MakeArchive()
m.chdir(main_directory)

for d in ["gfx","snd"]:
    m.add_directory(d,os.path.join(main_directory,d))

m.add_file("xenon1.exe",main_directory)
for dll in ["SDL.dll","SDL_image.dll","zlib1.dll","libjpeg-8.dll"]:
    src = which.which(dll)[0]
    m.add_file(src,main_directory)

m.add_file("readme.txt",main_directory)
m.add_buffer_as_file("xenon1_enhanced.bat",runenhanced,main_directory,text_mode=True)
m.create_zip("../Xenon1.zip")
