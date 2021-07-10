import os

text = ""

with open(os.getcwd()+"./faces.txt", "r") as file:
    text = file.read()

new_text = text.split("f ")

new_text_list = []

for a in new_text:
    b = a.split(" ")
    b[2] = b[2][:-1]
    new_text_list.append(b)

result = "{{"
for a in new_text_list:
    for b in range(len(a)):
        add = a[b].split("/")
        number = int(add[0])
        result += str(number-1)
        if b != 2:
            result += ","

    result += "},\n{"
result += "}}"

with open(os.getcwd()+"./new_faces.txt", "w") as file:
    file.write(result)
