#Python Code.


def testGraphName():
	return Graph1.property("name")

def testGraphPropertyName():
	return Graph1.name

def testNumberOfNodes():
	node = Graph1.nodes()
	print type(node)
	return len(node)

def getANode():
	node = a
	print type(a)
	return a.property("name")

