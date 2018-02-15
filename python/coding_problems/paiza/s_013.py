# 理想のタイムトラベル
# unfinished

def main(input):
	lines = input.split('\n')

	def read_line():
		return lines.pop(0)

	timeline_size = int(read_line())
	cost_future, cost_back = [int(x) for x in read_line().split()]
	timeline_status = list(read_line())

