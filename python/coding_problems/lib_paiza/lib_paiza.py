def get_paiza_input():
	lines = []
	while (True):
		try:
			lines.append(input())
		except:
			break
	return "\n".join(lines)