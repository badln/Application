#CAMERA

cm Main
cmm true
cmp -75.635887f, 22.039347f, 85.790657f
cmcc 0.1f, 0.1f, 0.1f, 0.1f
cmfov 75.0f
cmy 4.899979f
cmpt 4.100014f
cmfcp 500.0f
cmncp 0.1f

#OBJECT FMP

o FMP

c 0
ma 0.0f, 0.0f, 0.0f
md 1.0f, 1.0f, 1.0f
mdt none
ms 0.0f, 0.0f, 0.0f
mst none
me 0
met none
msh 1.0f
mc 1.0f, 1.0f, 1.0f, 1.0f
mt none

m Objects/fmp/FMP.fbx
mtf true
p 1.0f, 3.0f, 3.0f
s 1.0f, 1.0f, 1.0f
r -90.0f, 0.0f, -90.0f

l false
rq 0
lin 0.027f
q 0.0028
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
p -22.7f, 21.4f, 61.1f

l true
d 0.004731f, -0.428938f, 0.903322f
co 17.5f
oco 20.5f
t 1
la 0.3f, 0.3f, 0.3f
ld 0.3f, 0.3f, 0.3f
lc 1.0f, 1.0f, 1.0f, 1.0f

sh litShader

#OBJECT light2

o light
p -26.187992f, 28.160755f, 99.132446f

l true
d 0.0f, -1.0f, 0.0f
co 17.5f
oco 20.5f
t 0
la 1.0f, 1.0f, 1.0f
ld 1.0f, 1.0f, 1.0f
lc 1.0f, 1.0f, 1.0f, 1.0f

sh litShader

#END