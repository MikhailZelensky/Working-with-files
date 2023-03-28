file_size = 4 * 1024 * 1024 + 1
filename = "fileA"
with open(filename, "wb") as f:
	f.write(b"\x00" * file_size)
with open(filename, "r+b") as f:
	f.seek(0)
	f.write(b"\x01")
	f.seek(10000)
	f.write(b"\x01")
	f.seek(-1, 2)
	f.write(b"\x01")
