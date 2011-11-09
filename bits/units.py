#!/usr/bin/env python
# -*- encoding: utf-8 -*-

from __future__ import print_function
import sys
from fractions import Fraction


def generate_macros():
	with file('defs.hpp', 'w') as f:
		sys.stdout = f
		
		print('#ifndef SI_DEFS_HPP_')
		print('#define SI_DEFS_HPP_')
		print('')
		print('')
		print('#include "si_value.hpp"')
		print('')
		print('')
		print('/** @file */')
		
		for unit in UNITS:
			print('')
			print('')
			print(unit.header_doc())
			print('//@{')
			
			for multiple in unit.multiples:
				print('/// %s in %s' % (unit.quantities[0].capitalize(), multiple.name_plural()))
				print('#define %s(ValueType)\t%s' % (multiple.macro_name(), multiple.macro_definition()))
	
			print('//@}')
		
		print('')
		print('')
		print('#endif /* SI_DEFS_HPP_ */')
		
		sys.stdout = sys.__stdout__
		

def generate_units_header():
	with file('units.hpp', 'w') as f:
		sys.stdout = f
		
		print('#ifndef SI_UNITS_HPP_')
		print('#define SI_UNITS_HPP_')
		print('')
		print('')
		print('#include "defs.hpp"')
		print('')
		print('')
		print('namespace si {')
		print('')
		print('/// Pre-defined unit values that help to create other values.')
		print('namespace units {')
		
		for unit in UNITS:
			print('')
			print('')
			print(unit.header_doc())
			print('//@{')
			
			for multiple in unit.multiples:
				print('/// 1 %s (1 %s)' % (multiple.symbol(), multiple.name()))
				print('extern const %s\t%s;' % (multiple.const_declaration(), multiple.clean_symbol()))
						
			print('//@}')
		
		print('')
		print('')
		print('} /* namespace si::units */')
		print('} /* namespace si */')
		print('')
		print('')
		print('#endif /* SI_UNITS_HPP_ */')
		
		sys.stdout = sys.__stdout__
		

def generate_units():
	with file('units.cpp', 'w') as f:
		sys.stdout = f
		
		print('#include "units.hpp"')
		print('')
		print('')
		print('namespace si {')
		print('namespace units {')
		print('')
		
		
		for unit in UNITS:
			print('')
			
			for multiple in unit.multiples:
				print('const %s\t%s(1);' % (multiple.const_declaration(), multiple.clean_symbol()))
		
		print('')
		print('')
		print('} /* namespace si::units */')
		print('} /* namespace si */')
		
		sys.stdout = sys.__stdout__
		

def main():
	generate_macros()
	generate_units_header()
	generate_units()


################################################################################

UNITS = []
UNITS_BY_SYMBOL = {}

ALL_MULTIPLES = {}


class Prefix(object):
	def __init__(self, symbol):
		self.symbol = symbol

PREFIXES = {
		'pico' : Prefix('p'),
		'nano' : Prefix('n'),
		'micro': Prefix('μ'),
		'milli': Prefix('m'),
		'centi': Prefix('c'),
		'hecto': Prefix('h'),
		'kilo' : Prefix('k'),
		'mega' : Prefix('M'),
		'giga' : Prefix('G'),
		'tera' : Prefix('T'),
}


SELF = object()



class UnitMultiple(object):
	
	def __init__(self, unit, symbol, name, name_plural, definition_str):
		self.unit = unit
		self._symbol = symbol
		self._name = name
		self._name_plural = name_plural
		self.definition_str = definition_str
	
	def symbol(self):
		return self._symbol
	
	def name(self):
		return self._name
	
	def name_plural(self):
		return self._name_plural
	
	def clean_symbol(self):
		REPLACEMENT = {
				'μ' : 'u',
				'Ω' : 'ohm',
				'·' : '',
				'/' : '_',
				'²' : '2',
				'³' : '3',
				'(' : '',
				')' : '',
		}
		
		symbol = self._symbol
		for key, val in REPLACEMENT.iteritems():
			symbol = symbol.replace(key, val)
		return symbol
	
	def type_name(self):
		name_parts = self.unit.quantities[0].split(' ')
		type_name = ''.join(name_part.capitalize() for name_part in name_parts)
		return type_name + '_' + self.clean_symbol()
	
	def macro_name(self):
		return 'SI_%s_%s' % (self.unit.quantities[0].replace(' ', '').upper(), self.clean_symbol())
	
	def macro_definition(self):
		return self.definition_str
		
	def const_declaration(self):
		return self.macro_name() + '(int)'
	
	def definition_from(self):
		return '%s(ValueType)' % self.macro_name()



class Unit(object):
	
	def __init__(self, is_base_unit, quantities, symbol, definition_symbol):
		self._is_base_unit = is_base_unit
		self.quantities = quantities
		self.symbol = symbol
		self.definition_symbol = definition_symbol
		
		UNITS.append(self)
		UNITS_BY_SYMBOL[symbol] = self
	
	def process_multiples(self, multiples_specs, main_multiple):
		if multiples_specs is None:
			self.multiples = [main_multiple]
			ALL_MULTIPLES[main_multiple.symbol()] = main_multiple
		else:
			self.multiples = []
			for multiple_spec in multiples_specs:
				if multiple_spec is SELF:
					multiple = main_multiple
				elif isinstance(multiple_spec, str):
					prefix = multiple_spec
					p = PREFIXES[prefix]
					definition_from = main_multiple.definition_from()
					multiple = UnitMultiple(
									unit=self,
									symbol=p.symbol + self.symbol,
									name=prefix + self.name(),
									name_plural=prefix + self.name_plural(),
									definition_str='%s::apply_ratio< ::std::%s>::type' % (definition_from, prefix),
								)
				elif isinstance(multiple_spec, Ratio):
					ratio = multiple_spec
					definition_from = main_multiple.definition_from()
					if isinstance(ratio.multiplier, Fraction):
						ratio_str = '%d, %d' % (ratio.multiplier.numerator, ratio.multiplier.denominator)
					else:
						ratio_str = int(ratio.multiplier)
					multiple = UnitMultiple(
									unit=self,
									symbol=ratio.symbol,
									name=ratio.name,
									name_plural=ratio.name + 's',
									definition_str='%s::apply_ratio< ::std::ratio<%s>>::type' % (definition_from, ratio_str),
								)
				elif isinstance(multiple_spec, Definition):
					definition = multiple_spec
					multiple = UnitMultiple(
									unit=self,
									symbol=definition.symbol(),
									name=definition.name(),
									name_plural=definition.name_plural(),
									definition_str=definition.str()
								)
				else:
					raise NotImplementedError(multiple_spec)
	
				self.multiples.append(multiple)
				ALL_MULTIPLES[multiple.symbol()] = multiple
	
	
	def header_doc(self):
		if self.definition_symbol is None:
			def_symbol = ''
		else:
			def_symbol = ' = ' + self.definition_symbol
		
		return '\n'.join([
				'/**',
				' * @name %s' % self.quantities_str().capitalize(),
				' * %s unit: %s<br>' % ('Base' if self._is_base_unit else 'Derived', self.name()),
				' * Symbol: %s%s' % (self.symbol, def_symbol),
				' */'
			])
	
	def quantities_str(self):
		return ', '.join(self.quantities)



class BaseUnit(Unit):
	
	def __init__(self, index, quantities, symbol, name, multiples=None):
		super(BaseUnit, self).__init__(True, quantities, symbol, definition_symbol=None)
		
		self._name = name
		self._name_plural = name + 's'
		
		dimensions = [0] * 7
		dimensions[index] = 1
		
		main_multiple = UnitMultiple(
							unit=self,
							symbol=symbol,
							name=name,
							name_plural=name + 's',
							definition_str='::si::SIValue<ValueType, ::std::ratio<1>, %s>' % ', '.join(str(power) for power in dimensions)
						)
		
		multiples_specs = multiples
		self.process_multiples(multiples_specs, main_multiple)
	
	
	def name(self):
		return self._name
		
	def name_plural(self):
		return self._name_plural
		


class DerivedUnit(Unit):
	def __init__(self, quantities, definition, symbol=None, name=None, name_plural=None, multiples=None):
		if symbol is None:
			symbol = definition.symbol()
			definition_symbol = None
		else:
			definition_symbol = definition.symbol()
		super(DerivedUnit, self).__init__(False, quantities, symbol, definition_symbol)
		
		if name is None:
			name        = definition.name()
			name_plural = definition.name_plural()
		elif name_plural is None:
			name_plural = name + 's'
		
		self.definition = definition
		self._name = name
		self._name_plural = name_plural
		
		main_multiple = UnitMultiple(
							unit=self,
							symbol=symbol,
							name=name,
							name_plural=name_plural,
							definition_str=definition.str()
						)
		
		multiples_specs = multiples
		self.process_multiples(multiples_specs, main_multiple)
	
	def name(self):
		return self._name
	
	def name_plural(self):
		return self._name_plural



class Ratio(object):
	def __init__(self, multiplier, symbol, name):
		self.multiplier = multiplier
		self.symbol = symbol
		self.name = name



class Definition(object):
	def name_plural(self):
		return self.name() + 's'
	
	def definition_from(self):
		symbol = self.symbol()
		if symbol in ALL_MULTIPLES:
			return ALL_MULTIPLES[symbol].definition_from()
		else:
			return self.str()


class BinaryOperation(Definition):
	def __init__(self, operand1, operand2):
		self._operand1 = operand1
		self._operand2 = operand2
	
	def operand1(self):
		return ALL_MULTIPLES[self._operand1] if isinstance(self._operand1, str) else self._operand1
	
	def operand2(self):
		return ALL_MULTIPLES[self._operand2] if isinstance(self._operand2, str) else self._operand2


class Multiplication(BinaryOperation):
	def __init__(self, from_symbol1, from_symbol2):
		super(Multiplication, self).__init__(from_symbol1, from_symbol2)
	
	def symbol(self):
		return self.operand1().symbol() + '·' + self.operand2().symbol()
	
	def name(self):
		return self.operand1().name() + ' ' + self.operand2().name()
	
	def str(self):
		type1 = self.operand1().definition_from()
		type2 = self.operand2().definition_from()
		return '::si::multiplication< %s, %s>::type' % (type1, type2)


class Square(Multiplication):
	def __init__(self, from_symbol):
		super(Square, self).__init__(from_symbol, from_symbol)
		self.from_symbol = from_symbol
	
	def symbol(self):
		return self.from_symbol + '²'
	
	def name(self):
		multiple = ALL_MULTIPLES[self.from_symbol]
		return 'square ' + multiple.name()
	

class Cube(Multiplication):
	def __init__(self, from_symbol):
		super(Cube, self).__init__(Square(from_symbol).symbol(), from_symbol)
		self.from_symbol = from_symbol
	
	def symbol(self):
		return self.from_symbol + '³'
	
	def name(self):
		multiples = ALL_MULTIPLES[self.from_symbol]
		return 'cubic ' + multiples.name()
	

class Division(BinaryOperation):
	def __init__(self, operand1, operand2):
		super(Division, self).__init__(operand1, operand2)
	
	def symbol(self):
		if self._operand1 is 1:
			symbol1 = '1'
		else:
			symbol1 = self.operand1().symbol()
		
		symbol2 = self.operand2().symbol()
		if '·' in symbol2:
			symbol2 = '(' + symbol2 + ')'
		
		return symbol1 + '/' + symbol2
	
	def name(self):
		return self.operand1().name() + ' per ' + self.operand2().name()
	
	def name_plural(self):
		return self.operand1().name_plural() + ' per ' + self.operand2().name()
	
	def str(self):
		if self._operand1 is 1:
			t1 = 'ValueType'
		else:
			t1 = self.operand1().definition_from()
		
		return '::si::division< %s, %s>::type' % (t1, self.operand2().definition_from())


################################################################################


BaseUnit(0,
	quantities=['length', 'distance'],
	symbol='m',
	name='meter',
	multiples=['nano', 'micro', 'milli', 'centi', SELF, 'kilo']
)

BaseUnit(1,
	quantities=['mass'],
	symbol='g',
	name='gram',
	multiples=['pico', 'nano', 'micro', 'milli', 'centi', SELF, 'kilo']
)

BaseUnit(2,
	quantities=['time'],
	symbol='s',
	name='second',
	multiples=['nano', 'micro', 'milli', SELF,
			Ratio(      60, symbol='min', name='minute'),
			Ratio(   60*60, symbol='h'  , name='hour'),
			Ratio(24*60*60, symbol='d'  , name='day'),
		]
)

BaseUnit(3,
	quantities=['electric current'],
	symbol='A',
	name='ampere',
	multiples=['milli', SELF]
)

BaseUnit(4,
	quantities=['temperature'],
	symbol='K',
	name='kelvin'
)

BaseUnit(5,
	quantities=['luminous intensity'],
	symbol='cd',
	name='candela'
)

BaseUnit(6,
	quantities=['amount of substance'],
	symbol='mol',
	name='mole'
)

DerivedUnit(
	quantities=['area'],
	definition=Square('m'),
	multiples=[Square('cm'), SELF, Square('km')]
)

DerivedUnit(
	quantities=['volume'],
	definition=Cube('m'),
)

DerivedUnit(
	quantities=['speed', 'velocity'],
	definition=Division('m', 's'),
	multiples=[SELF, Division('km', 'h')]
)

DerivedUnit(
	quantities=['acceleration'],
	definition=Division('m', Square('s'))
)

DerivedUnit(
	quantities=['force', 'weight'],
	symbol='N',
	name='newton',
	definition=Multiplication('kg', 'm/s²'),
)

DerivedUnit(
	quantities=['pressure', 'stress'],
	symbol='Pa',
	name='pascal',
	definition=Division('N', 'm²'),
	multiples=[SELF, 'hecto', 'kilo', 'mega', 'giga']
)

DerivedUnit(
	quantities=['energy', 'work', 'heat'],
	symbol='J',
	name='joule',
	definition=Multiplication('N', 'm'),
	multiples=['pico', 'nano', 'micro', 'milli', SELF, 'kilo', 'mega', 'giga', 'tera']
)

DerivedUnit(
	quantities=['power'],
	symbol='W',
	name='watt',
	definition=Division('J', 's'),
	multiples=['pico', 'nano', 'micro', 'milli', SELF, 'kilo', 'mega', 'giga', 'tera']
)

DerivedUnit(
	quantities=['electric charge'],
	symbol='C',
	name='coulomb',
	definition=Multiplication('A', 's'),
	multiples=['pico', 'nano', 'micro', 'milli', SELF],
)

DerivedUnit(
	quantities=['voltage', 'electrical potential difference'],
	symbol='V',
	name='volt',
	definition=Division('W', 'A'),
	multiples=['milli', SELF, 'kilo', 'mega']
)

DerivedUnit(
	quantities=['electric capacitance'],
	symbol='F',
	name='farad',
	definition=Division('C', 'V'),
	multiples=['pico', 'nano', 'micro', SELF]
)

DerivedUnit(
	quantities=['electric resistance', 'impedance', 'reactance'],
	symbol='Ω',
	name='ohm',
	definition=Division('V', 'A'),
)

DerivedUnit(
	quantities=['electrical conductance'],
	symbol='S',
	name='siemens', name_plural='siemens',
	definition=Division('A', 'V'),
)

DerivedUnit(
	quantities=['magnetic flux'],
	symbol='Wb',
	name='weber',
	definition=Division('J', 'A'),
	multiples=[Ratio(Fraction(1, 100000000), 'Mx', 'maxwell'), SELF]
)

DerivedUnit(
	quantities=['magnetic field strength', 'magnetic flux density'],
	symbol='T',
	name='tesla',
	definition=Division('Wb', 'm²'),
	multiples=['nano', 'micro', 'milli', SELF]
)

DerivedUnit(
	quantities=['inductance'],
	symbol='H',
	name='henry', name_plural='henries',
	definition=Division('Wb', 'A'),
)

DerivedUnit(
	quantities=['equivalent dose'],
	symbol='Sv',
	name='sievert',
	definition=Division('J', 'kg'),
	multiples=['micro', 'milli', SELF]
)

DerivedUnit(
	quantities=['catalytic activity'],
	symbol='kat',
	name='katal',
	definition=Division('mol', 's'),
)

DerivedUnit(
	quantities=['frequency'],
	symbol='Hz',
	name='hertz', name_plural='hertz',
	definition=Division(1, 's')
)

DerivedUnit(
	quantities=['volumetric flow'],
	definition=Division(Cube('m'), 's')
)

DerivedUnit(
	quantities=['momentum', 'impulse'],
	definition=Multiplication('N', 's')
)

DerivedUnit(
	quantities=['torque', 'moment of force'],
	definition=Multiplication('N', 'm')
)

DerivedUnit(
	quantities=['angular momentum'],
	definition=Multiplication('N·m', 's')
)

DerivedUnit(
	quantities=['yank'],
	definition=Division('N', 's')
)

DerivedUnit(
	quantities=['area density'],
	definition=Division('kg', 'm²')
)

DerivedUnit(
	quantities=['density', 'mass density'],
	definition=Division('kg', 'm³')
)

DerivedUnit(
	quantities=['specific volume'],
	definition=Division('m³', 'kg')
)

DerivedUnit(
	quantities=['amount of substance concentration'],
	definition=Division('mol', 'm³')
)

DerivedUnit(
	quantities=['molar volume'],
	definition=Division('m³', 'mol')
)

DerivedUnit(
	quantities=['action'],
	definition=Multiplication('J', 's')
)

DerivedUnit(
	quantities=['heat capacity', 'entropy'],
	definition=Division('J', 'K')
)

DerivedUnit(
	quantities=['molar heat capacity', 'molar entropy'],
	definition=Division('J', Multiplication('K', 'mol'))
)

DerivedUnit(
	quantities=['specific heat capacity', 'specific entropy'],
	definition=Division('J', Multiplication('K', 'kg'))
)

DerivedUnit(
	quantities=['molar energy'],
	definition=Division('J', 'mol')
)

DerivedUnit(
	quantities=['specific energy'],
	definition=Division('J', 'kg')
)

DerivedUnit(
	quantities=['energy density'],
	definition=Division('J', 'm³')
)

DerivedUnit(
	quantities=['surface tension'],
	definition=Division('N', 'm')
)

DerivedUnit(
	quantities=['heat flux density', 'irradiance'],
	definition=Division('W', 'm²')
)

DerivedUnit(
	quantities=['thermal conductivity'],
	definition=Division('W', Multiplication('m', 'K'))
)

DerivedUnit(
	quantities=['kinematic viscosity', 'diffusion coefficient'],
	definition=Division('m²', 's')
)

DerivedUnit(
	quantities=['dynamic viscosity'],
	definition=Multiplication('Pa', 's')
)

DerivedUnit(
	quantities=['electric displacement field', 'polarization vector'],
	definition=Division('C', 'm²')
)

DerivedUnit(
	quantities=['electric charge density'],
	definition=Division('C', 'm³')
)

DerivedUnit(
	quantities=['electric current density'],
	definition=Division('A', 'm²')
)

DerivedUnit(
	quantities=['conductivity'],
	definition=Division('S', 'm')
)

DerivedUnit(
	quantities=['molar conductivity'],
	definition=Division(Multiplication('S', 'm²'), 'mol')
)

DerivedUnit(
	quantities=['permittivity'],
	definition=Division('F', 'm')
)

DerivedUnit(
	quantities=['permeability'],
	definition=Division('H', 'm')
)

DerivedUnit(
	quantities=['electric field strength'],
	definition=Division('V', 'm')
)

DerivedUnit(
	quantities=['magnetic field strength'],
	definition=Division('A', 'm')
)

DerivedUnit(
	quantities=['luminance'],
	definition=Division('cd', 'm²')
)

DerivedUnit(
	quantities=['exposure'],
	definition=Division('C', 'kg')
)

DerivedUnit(
	quantities=['resistivity'],
	definition=Multiplication('Ω', 'm')
)



if __name__ == '__main__':
	main()
