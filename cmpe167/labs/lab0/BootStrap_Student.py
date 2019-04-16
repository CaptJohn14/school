import pip

PackagesNeeded=['requests']
for package in PackagesNeeded:
	print('Installing '+package)
	pip.main(['install','--upgrade',package])