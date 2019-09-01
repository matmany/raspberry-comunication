pipes = ["1Node", "2Node", "3Node"]
print(hex(pipes[0]))
hexaPipes = []
for pipe in pipes:
    hexaCode = []
    for letter in pipe:
        hexaCode.append(letter.encode("hex"))

    hexaPipes.append(hexaCode)
print("hexaPipes")


