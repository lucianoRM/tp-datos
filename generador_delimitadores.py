

def generar_delimitadores():
	archivo = open('delimitadores.txt','w');
	for x in xrange(65):
		archivo.write(chr(x))
		archivo.write(',')
	for x in xrange(91,97):
		archivo.write(chr(x))
		archivo.write(',')
	for x in xrange(123,127):
		archivo.write(chr(x))
		archivo.write(',')
	archivo.close()

generar_delimitadores()
