import os

text = ""

with open(os.getcwd()+"./normals.txt", "r") as file:
    text = file.read()

new_text = text.split("vn ")

new_text_normals = []

for a in new_text:
    b = a.split(" ")
    b[2] = b[2][:-1]
    new_text_normals.append(b)

result_normals=new_text_normals[:]

#tris
with open(os.getcwd()+"./faces.txt", "r") as file:
    text = file.read()

new_text = text.split("f ")

new_text_list = []

for a in new_text:
    b = a.split(" ")
    b[2] = b[2][:-1]
    new_text_list.append(b)

for a in new_text_list:
    for b in range(len(a)):
        add = a[b].split("/")
        
        result_normals[int(add[0])-1]=new_text_normals[int(add[2])-1]

#tris

result = "{{"

for a in result_normals:
    for b in range(len(a)):
        result += a[b]
        if b != 2:
            result += "f,"
        else:
            result+="f"

    result +="},\n{"
result += "}}"
with open(os.getcwd()+"./new_normals.txt", "w") as file:
    file.write(result)
