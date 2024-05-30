# The goal is to make the handling of the memmap much less of a hassle.
# This can be done by removing fluff

# Open the memmap file
from signal import siginterrupt

# Open large map from makefile output
Largemap = open("/mnt/v/boot/memmap/memmap", 'rt')

# used to count bin items
binCount = 0
Bin = 0
binItems = 75

# List of bins
binData = []
binList = []

# Header
headerData = []

# Remove 'broken' syms
# and clean up 0's
for line in Largemap:

	# Split the line by spaces
	splitLines = line.split(' ', 3)

	# Declare address and size variables
	addr = 0
	size = 0

	# Try to get int
	try:
		addr = int(splitLines[0], 16)
		size = int(splitLines[1], 16)
	# On fail, just keep going
	except:
		continue

	# Get name
	name = splitLines[3]

	# Remove any newlines
	name = name.replace('\n', '')

	# Check if name contains a '(' or not. If not, discard unless the function is 'main'
	if '(' not in name and name != "main":
		continue

	# If name contains '__' or 'gnu' or 'cxx', discard
	if "__" in name or "gnu" in name or "cxx" in name:
		continue

	# If name starts with 'std' discard
	if name.startswith("std"):
		continue

	# Get address and size in hex
	addrHex = "%0.6x" % addr
	sizeHex = "%0.4x" % size

	# Calculate bin items accordingly
	binCount += 1

	# Reset if necessary
	if binCount >= binItems:

		# Clear binCount
		binCount = 0
		Bin += 1

		# Append BinList
		binList.append(binData)

		# Clear binData
		binData = []

	# Add name to binList
	binData.append(name)

	# create header and save
	header = f"{addrHex},{sizeHex}?{Bin}"
	headerData.append(header)

# Save header data
with open('/mnt/v/boot/memmap/memmaphdr', 'w') as outfile:
  outfile.write('\n'.join(str(i) for i in headerData))

Bin = 0

# Then for each other bin
for BinDat in binList:
	with open(f'/mnt/v/boot/memmap/memmap.{Bin}', 'w') as outfile:
		outfile.write('\n'.join(str(i) for i in BinDat))
	Bin += 1
