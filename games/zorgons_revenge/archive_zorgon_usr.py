import os, make_archive_no_cc as make_archive,which

runenhanced="""
xenon1 -e
"""

main_directory = "ZorgonsRevenge"

m = make_archive.MakeArchive()
m.chdir(main_directory)

for d in ["gfx","snd"]:
    m.add_directory(d,os.path.join(main_directory,d))

m.add_file("zorgon.exe",main_directory)
m.add_file("readme.txt",main_directory)
for dll in ["SDL.dll","SDL_image.dll","zlib1.dll","libjpeg-8.dll"]:
    src = which.which(dll)[0]
    m.add_file(src,main_directory)

m.add_file("readme.txt",main_directory)
m.add_buffer_as_file("zorgon_fullscreen_enh.bat",runenhanced,main_directory,text_mode=True)
m.create_zip("../ZorgonsRevenge.zip")
