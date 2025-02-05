#import importlib.util
#import sys
#import pathlib
import os
import time

#python_module_path = pathlib.Path(os.path.abspath(os.curdir)+'/build/RelWithDebInfo')
#sys.path.append(str(python_module_path))
print(os.getpid())

breakpoint()
import cpy # import module


breakpoint()
result = cpy.add((2,2))

print(f"result of summary: {result}")