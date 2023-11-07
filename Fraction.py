##############
## EXAMPLE: simple class to represent fractions
## Try adding more built-in operations like multiply, divide
## Try adding a reduce method to reduce the fraction (use gcd)

class Fraction(object):
	"""
	A number represented as a fraction
	"""
	def __init__(self, num, denom):
		""" num and denom are integers """
		assert type(num) == int and type(denom) == int, "ints not used"
		
		self.num 	= num
		self.denom 	= denom
	
	def __str__(self):
		""" Returns a string representation of self """
		return str(self.num) + "/" + str(self.denom)
		
	def add(self, other):
		""" Returns a new fraction representing the addition """
		new_num  	= self.num*other.denom + self.denom*other.num
		new_denom  	= self.denom*other.denom
		return Fraction(new_num, new_denom)
		
	def sub(self, other):
		""" Returns a new fraction representing the subtraction """
		new_num  	= self.num*other.denom - self.denom*other.num
		new_denom  	= self.denom*other.denom
		return Fraction(new_num, new_denom)
		