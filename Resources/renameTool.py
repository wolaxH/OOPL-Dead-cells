import os
from colorama import Fore, Style
from natsort import natsorted

#Plaese check that your res are sorted, this Tool has no sort function
"""
    you have to install natsort. 
    if you dont have, you can use terminal and type "pip install natsort" to install
"""

Path = os.getcwd().replace("\\", "/") + "/Zombie/move"   #The folder path that you will replac

prefix = "move_"    #The new name prefix eg: runB_{index}.png
errorItem = []

for i, old_name in enumerate(natsorted(os.listdir(Path)), start = 0):
    old_path = os.path.join(Path, old_name)
    new_name = f"{prefix}{i}{os.path.splitext(old_name)[1]}"
    new_path = os.path.join(Path, new_name)

    try:
        os.rename(old_path, new_path)
        print(f"Rename {Fore.YELLOW + old_name + Style.RESET_ALL} to {Fore.YELLOW + new_name + Style.RESET_ALL} successes!")
    except:
        errorItem.append(old_name)

if (len(errorItem)):
    print(f"\n{len(errorItem)} occur")
    print("\n".join(item for item in errorItem))
else:
    print("complete with no error")