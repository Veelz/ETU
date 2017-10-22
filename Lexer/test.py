import subprocess
print("Running with -filter")
str = "bin/lexer.exe -filter testfiles/comment.txt"
p = subprocess.call( str.split() )
print("Running without -filter")
str = "bin/lexer.exe -filter testfiles/comment.txt"
p = subprocess.call( str.split() )
