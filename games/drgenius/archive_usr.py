import os, make_archive_no_cc as make_archive,which



m = make_archive.MakeArchive()


m.add_file("drgenius.exe")
for dll in ["SDL.dll","SDL_image.dll","zlib1.dll","libjpeg-8.dll"]:
    src = which.which(dll)[0]
    m.add_file(src)

m.add_directory("snd")
m.add_file("readme.txt")
#m.add_buffer_as_file("strip21.bat",run,main_directory,text_mode=True)
m.create_zip(r"D:\jff\data\webpage\conversions\DrGenius.zip")
