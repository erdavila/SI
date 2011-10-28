#!/usr/bin/env python
# -*- encoding: utf-8 -*-

from __future__ import print_function
import sys


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
			
			for variant in unit.variants:
				print('/// %s in %s' % (unit.quantities[0].capitalize(), variant.name_plural()))
				print('#define %s(ValueType)\t%s' % (variant.macro_name(), variant.macro_definition()))
	
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
			
			for variant in unit.variants:
				print('/// 1%s (1 %s)' % (variant.symbol(), variant.name()))
				print('extern const %s\t%s;' % (variant.const_declaration(), variant.clean_symbol()))
						
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
			
			for variant in unit.variants:
				print('const %s\t%s(1);' % (variant.const_declaration(), variant.clean_symbol()))
		
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

VARIANTS = {}


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



class UnitVariant(object):
	
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
		return self._symbol.replace('μ', 'u').replace('/', '_').replace('²', '2').replace('³', '3')
	
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
	
	def __init__(self, is_base_unit, quantities, symbol):
		self._is_base_unit = True
		self.quantities = quantities
		self.symbol = symbol
		
		UNITS.append(self)
		UNITS_BY_SYMBOL[symbol] = self
	
	def process_variants(self, variants_specs, main_variant):
		if variants_specs is None:
			self.variants = [main_variant]
			VARIANTS[main_variant.symbol()] = main_variant
		else:
			self.variants = []
			for variant_spec in variants_specs:
				if variant_spec is SELF:
					variant = main_variant
				elif isinstance(variant_spec, str):
					prefix = variant_spec
					p = PREFIXES[prefix]
					definition_from = main_variant.definition_from()
					variant = UnitVariant(
									unit=self,
									symbol=p.symbol + self.symbol,
									name=prefix + self.name(),
									name_plural=prefix + self.name_plural(),
									definition_str='%s::apply_ratio< ::std::%s>::type' % (definition_from, prefix),
								)
				elif isinstance(variant_spec, Ratio):
					ratio = variant_spec
					definition_from = main_variant.definition_from()
					variant = UnitVariant(
									unit=self,
									symbol=ratio.symbol,
									name=ratio.name,
									name_plural=ratio.name + 's',
									definition_str='%s::apply_ratio< ::std::ratio<%d>>::type' % (definition_from, ratio.multiplier),
								)
				elif isinstance(variant_spec, Definition):
					definition = variant_spec
					variant = UnitVariant(
									unit=self,
									symbol=definition.symbol(),
									name=definition.name(),
									name_plural=definition.name_plural(),
									definition_str=definition.str()
								)
				else:
					raise NotImplementedError(variant_spec)
	
				self.variants.append(variant)
				VARIANTS[variant.symbol()] = variant
	
	
	def header_doc(self):
		return '\n'.join([
				'/**',
				' * @name %s' % self.quantities_str().capitalize(),
				' * %s unit: %s<br>' % ('Base' if self._is_base_unit else 'Derived', self.name()),
				' * Symbol: %s' % self.symbol,
				' */'
			])
	
	def quantities_str(self):
		return ', '.join(self.quantities)



class BaseUnit(Unit):
	
	def __init__(self, index, quantities, symbol, name, variants=None):
		super(BaseUnit, self).__init__(True, quantities, symbol)
		
		self._name = name
		self._name_plural = name + 's'
		
		base_unit_powers = [0] * 7
		base_unit_powers[index] = 1
		
		main_variant = UnitVariant(
							unit=self,
							symbol=symbol,
							name=name,
							name_plural=name + 's',
							definition_str='::si::SIValue<ValueType, ::std::ratio<1>, %s>' % ', '.join(str(power) for power in base_unit_powers)
						)
		
		variants_specs = variants
		self.process_variants(variants_specs, main_variant)
	
	
	def name(self):
		return self._name
		
	def name_plural(self):
		return self._name_plural
		


class DerivedUnit(Unit):
	def __init__(self, quantities, definition, symbol=None, name=None, name_plural=None, variants=None):
		if symbol is None: symbol = definition.symbol()
		super(DerivedUnit, self).__init__(False, quantities, symbol)
		
		if name is None:
			name        = definition.name()
			name_plural = definition.name_plural()
		elif name_plural is None:
			name_plural = name + 's'
		
		self.definition = definition
		self._name = name
		self._name_plural = name_plural
		
		main_variant = UnitVariant(
							unit=self,
							symbol=symbol,
							name=name,
							name_plural=name_plural,
							definition_str=definition.str()
						)
		
		variants_specs = variants
		self.process_variants(variants_specs, main_variant)
	
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
		if symbol in VARIANTS:
			return VARIANTS[symbol].definition_from()
		else:
			return self.str()


class BinaryOperation(Definition):
	def __init__(self, operand1, operand2):
		self._operand1 = operand1
		self._operand2 = operand2
	
	def operand1(self):
		return VARIANTS[self._operand1] if isinstance(self._operand1, str) else self._operand1
	
	def operand2(self):
		return VARIANTS[self._operand2] if isinstance(self._operand2, str) else self._operand2


class Multiplication(BinaryOperation):
	def __init__(self, from_symbol1, from_symbol2):
		super(Multiplication, self).__init__(from_symbol1, from_symbol2)
	
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
		variant = VARIANTS[self.from_symbol]
		return 'square ' + variant.name()
	

class Cube(Multiplication):
	def __init__(self, from_symbol):
		super(Cube, self).__init__(Square(from_symbol).symbol(), from_symbol)
		self.from_symbol = from_symbol
	
	def symbol(self):
		return self.from_symbol + '³'
	
	def name(self):
		variant = VARIANTS[self.from_symbol]
		return 'cubic ' + variant.name()
	

class Division(BinaryOperation):
	def __init__(self, operand1, operand2):
		super(Division, self).__init__(operand1, operand2)
	
	def symbol(self):
		return self.operand1().symbol() + '/' + self.operand2().symbol()
	
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
	variants=['nano', 'micro', 'milli', 'centi', SELF, 'kilo']
)

BaseUnit(1,
	quantities=['mass'],
	symbol='g',
	name='gram',
	variants=['pico', 'nano', 'micro', 'milli', 'centi', SELF, 'kilo']
)

BaseUnit(2,
	quantities=['time'],
	symbol='s',
	name='second',
	variants=['nano', 'micro', 'milli', SELF,
			Ratio(      60, symbol='min', name='minute'),
			Ratio(   60*60, symbol='h'  , name='hour'),
			Ratio(24*60*60, symbol='d'  , name='day'),
		]
)

BaseUnit(3,
	quantities=['electric current'],
	symbol='A',
	name='ampere',
	variants=['milli', SELF]
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
	variants=[Square('cm'), SELF, Square('km')]
)

DerivedUnit(
	quantities=['volume'],
	definition=Cube('m'),
)

DerivedUnit(
	quantities=['speed', 'velocity'],
	definition=Division('m', 's'),
	variants=[SELF, Division('km', 'h')]
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
	variants=[SELF, 'hecto', 'kilo', 'mega', 'giga']
)

DerivedUnit(
	quantities=['energy', 'work', 'heat'],
	symbol='J',
	name='joule',
	definition=Multiplication('N', 'm'),
	variants=['pico', 'nano', 'micro', 'milli', SELF, 'kilo', 'mega', 'giga', 'tera']
)

DerivedUnit(
	quantities=['power'],
	symbol='W',
	name='watt',
	definition=Division('J', 's'),
	variants=['pico', 'nano', 'micro', 'milli', SELF, 'kilo', 'mega', 'giga', 'tera']
)

DerivedUnit(
	quantities=['electric charge'],
	symbol='C',
	name='coulomb',
	definition=Multiplication('A', 's'),
	variants=['pico', 'nano', 'micro', 'milli', SELF],
)

DerivedUnit(
	quantities=['voltage', 'electrical potential difference'],
	symbol='V',
	name='volt',
	definition=Division('W', 'A'),
	variants=['milli', SELF, 'kilo', 'mega']
)

DerivedUnit(
	quantities=['electric capacitance'],
	symbol='F',
	name='farad',
	definition=Division('C', 'V'),
	variants=['pico', 'nano', 'micro', SELF]
)

DerivedUnit(
	quantities=['frequency'],
	symbol='Hz',
	name='hertz', name_plural='hertz',
	definition=Division(1, 's')
)


if __name__ == '__main__':
	main()
