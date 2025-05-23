#CAMERA

cm Main
cmm true
cmp 1.0f, 10.0f, 1.0f
cmcc 0.1f, 0.1f, 0.1f, 0.1f
cmfov 75.0f
cmy 20.0f
cmpt -45.0f
cmfcp 500.0f
cmncp 0.1f

#OBJECT hornet

o hornet

c 0
ma 0.1f, 0.1f, 0.1f
md 1.0f, 1.0f, 1.0f
mdt none
ms 0.5f, 0.5f, 0.5f
mst none
me 0
met none
msh 100.0f
mc 1.0f, 1.0f, 1.0f, 1.0f
mt none

m Objects/v1/source/model.gltf
mtf true
p 1.0f, 3.0f, 3.0f
s 1.0f, 1.0f, 1.0f
r 0.0f, 0.0f, 0.0f

l false
rq 0
lin 0.07f
q 0.017
cnst 1.0f
co 1.5f
oco 17.5f
t 0
d 0.0f, 0.0f, 0.0f
lc 1.0f, 1.0f, 1.0f, 0.0f
lp 0.0f, 0.0f, 0.0f
la 1.0f, 1.0f, 1.0f
ld 1.0f, 1.0f, 1.0f
ls 1.0f, 1.0f, 1.0f

sh litShader

#OBJECT light

o light
p 0.0f, 6.0f, 1.0f

l true
d 0.0f, -1.0f, 0.0f
co 17.5f
oco 20.5f
t 1
la 1.0f, 1.0f, 1.0f
ld 1.0f, 1.0f, 1.0f
lc 1.0f, 1.0f, 1.0f, 1.0f

sh litShader

#OBJECT light

o light2
p 1.0f, 1.0f, 1.0f

l true
d 0.0f, -1.0f, 0.0f
co 17.5f
oco 20.5f
t 0
lc 1.0f, 1.0f, 1.0f, 1.0f

sh litShader

#END