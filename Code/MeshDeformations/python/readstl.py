import struct

normals = []
points = []
triangles = []
bytecount = []

fb = [] # debug list

def unpack (f, sig, l):
    s = f.read (l)
    fb.append(s)
    return struct.unpack(sig, s)

def read_triangle(f):
    n = unpack(f,"<3f", 12)
    p1 = unpack(f,"<3f", 12)
    p2 = unpack(f,"<3f", 12)
    p3 = unpack(f,"<3f", 12)
    b = unpack(f,"<h", 2)

    normals.append(n)
    l = len(points)
    points.append(p1)
    points.append(p2)
    points.append(p3)
    triangles.append((l, l+1, l+2))
    bytecount.append(b[0])


def read_length(f):
    length = struct.unpack("@i", f.read(4))
    print length
    return length[0]

def read_header(f):
    f.seek(f.tell()+80)

def write_as_ascii(outfilename):
    f = open(outfilename, "w")
    f.write ("solid "+outfilename+"\n")
    for n  in range(len(triangles)):
        f.write ("facet normal {} {} {}\n".format(normals[n][0],normals[n][1],normals[n][2]))
        f.write ("outer loop\n")
        f.write ("vertex {} {} {}\n".format(points[triangles[n][0]][0],points[triangles[n][0]][1],points[triangles[n][0]][2]))
        f.write ("vertex {} {} {}\n".format(points[triangles[n][1]][0],points[triangles[n][1]][1],points[triangles[n][1]][2]))
        f.write ("vertex {} {} {}\n".format(points[triangles[n][2]][0],points[triangles[n][2]][1],points[triangles[n][2]][2]))
        f.write ("endloop\n")
        f.write ("endfacet\n")
    f.write ("endsolid "+outfilename+"\n")
    f.close()

def main():
    infilename = r"stl/example.stl"
    outfilename = r"stl/example_ascii_test.stl"

    try:
        f = open ( infilename, "rb")

        read_header(f)
        l = read_length(f)
        try:
            while True:
                read_triangle(f)
        except Exception, e:
            print "Exception",e[0]
        print len(normals), len(points), len(triangles), l
        write_as_ascii(outfilename)

    except Exception, e:
        print e


if __name__ == '__main__':
    main()