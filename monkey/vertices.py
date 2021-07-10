import os

text = ""

with open(os.getcwd()+"./vertices.txt", "r") as file:
    text = file.read()

new_text = text.split("v ")

new_text_list = []

for a in new_text:
    b = a.split(" ")
    b[2] = b[2][:-1]
    new_text_list.append(b)

result = "{{"
for a in new_text_list:
    for b in range(len(a)):
        result += a[b]
        if b != 2:
            result += "f,"
        else:
            result+="f"

    result +="},\n{"
result += "}}"

with open(os.getcwd()+"./new_vertices.txt", "w") as file:
    file.write(result)
