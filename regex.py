import re
#p = re.compile("^.*,.*$")

map = []
counter = 0
f = open("source/tiles/MapTileType.hpp")
for line in f:
#    print('Line: ' + line)
#    print(p.match(line))
    if ',' in line:
        key = line[:line.index(',')]
        value = counter
        entry = {key, value}
        map.append(entry)
        counter += 1

for entry in map:
    print(str(entry))
