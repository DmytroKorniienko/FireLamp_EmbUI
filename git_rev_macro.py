import subprocess

try:
	revision = subprocess.check_output(["git", "rev-parse", "HEAD"]).strip()
	print ("-DGIT_SRC_REV=\\\"%s\\\"" % revision)
except:
	print ("-DGIT_SRC_REV=\\\"UNKNOWN\\\"")
