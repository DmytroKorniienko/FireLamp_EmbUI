import subprocess

try:
	revision = subprocess.check_output(["git", "rev-parse", "HEAD"]).strip()
	print ("-DPIO_SRC_REV=\\\"%s\\\"" % revision)
except:
	print ("-DPIO_SRC_REV=\\\"UNKNOWN\\\"")
