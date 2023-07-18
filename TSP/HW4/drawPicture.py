from  matplotlib import pyplot as plt
import sys

def findPointOrder(result,points,axis):
	result =  [int(index[1]) for index in sorted(dict(zip(result, points[axis])).items())]
	return result + [result[0]]

def main():
	with open(sys.argv[1]+"/ans_"+sys.argv[1]+".txt", 'r') as getOrder:
		order = getOrder.read().splitlines()[1:]

	result = [-1] * len(order)
	for i, j in zip(range(len(order)),order):
		result[int(j)-1] = i
	
	with open(sys.argv[1]+"/point.txt", 'r') as getPoint:
		points = list(zip(*[line.split(' ') for line in getPoint.read().splitlines()]))
	
	PointX,PointY = findPointOrder(result,points,1), findPointOrder(result,points,2)
	
	_, ax = plt.subplots()
	ax.plot(PointX, PointY, linestyle = '-', marker = '')
	ax.scatter(PointX, PointY)
	ax.set_title("HW4/"+sys.argv[1])
	plt.show()
	
if __name__ == "__main__":
	main()
