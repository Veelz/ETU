import sys
import subprocess

arg = sys.argv[1]
str = "bin/parsercom " + arg + " -print"
p = subprocess.call( str.split() )
