import os, make_archive_no_cc as make_archive,which

runenhanced="""
l_aigle_d_or -n
"""

main_directory = "L_Aigle_D_Or"

m = make_archive.MakeArchive()

for d in ["snd"]:
    m.add_directory(d,os.path.join(main_directory,d))

m.add_file("l_aigle_d_or.exe",main_directory)
for dll in ["SDL.dll"]:
    src = which.which(dll)[0]
    m.add_file(src,main_directory)

m.add_file("readme.txt",main_directory)
m.add_buffer_as_file("game_nointro.bat",runenhanced,main_directory,text_mode=True)
m.create_zip("../L_Aigle_D_Or.zip")
