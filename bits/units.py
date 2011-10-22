#!/usr/bin/env python
# -*- encoding: utf-8 -*-

from __future__ import print_function
import itertools
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
		
		for group in Group.GROUPS:
			print('')
			print('')
			print(group.header_doc())
			print('//@{')
			
			for unit in group.units:
				print('/// %s in %s' % (group.quantities[0].capitalize(), unit.plural))
				print(unit.macro_definition())
			
			print('//@}')
		
		print('')
		print('')
		print('#endif /* SI_DEFS_HPP_ */')
		
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
		
		
		for group in Group.GROUPS:
			print('')
			
			for unit in group.units:
				print(unit.const_declaration())
		
		print('')
		print('')
		print('} /* namespace si::units */')
		print('} /* namespace si */')
		
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
		
		for group in Group.GROUPS:
			print('')
			print('')
			print(group.header_doc())
			print('//@{')
			
			for unit in group.units:
				print('/// 1%s (1 %s)' % (unit.symbol, unit.singular))
				print(unit.extern_const_declaration())
						
			print('//@}')
		
		print('')
		print('')
		print('} /* namespace si::units */')
		print('} /* namespace si */')
		print('')
		print('')
		print('#endif /* SI_UNITS_HPP_ */')
		
		sys.stdout = sys.__stdout__
		

def main():
	generate_macros()
	generate_units()
	generate_units_header()



class Group(object):
	def __init__(self, is_base_unit, quantities, symbol, *units):
		self.is_base_unit = is_base_unit
		self.quantities = quantities
		if isinstance(quantities, str):
			self.quantities = [quantities]
		self.symbol = symbol
		self.units = units
		for unit in units:
			unit.group = self
		Group.GROUPS.append(self)
	
	def quantities_str(self):
		quantities = ', '.join(self.quantities)
		return quantities
	
	def header_doc(self):
		unit_name = Unit.UNITS[self.symbol].singular
		
		return '\n'.join([
				'/** @name %s' % self.quantities_str().capitalize(),
				' * %s unit: %s<br>' % ('Base' if self.is_base_unit else 'Derived', unit_name),
				' * Symbol: %s' % self.symbol,
				' */'
			])
	
	GROUPS = []


class Unit(object):
	def __init__(self, symbol, singular, plural, definition):
		self.symbol = symbol
		self.singular = singular
		self.plural = plural
		self.definition = definition
		if symbol in Unit.UNITS:
			raise Exception(symbol)
		Unit.UNITS[symbol] = self
	
	def macro(self, argument):
		quantity = self.group.quantities[0]
		if not isinstance(quantity, str):
			quantity = quantity[0]
		quantity = quantity.upper().replace(' ', '')
		
		return 'SI_%s_%s(%s)' % (quantity, self.clean_symbol(), argument)
	
	def macro_definition(self):
		definition = self.definition.fmt('VALUETYPE')
	
		return '#define %s\t%s' % (self.macro('VALUETYPE'), definition)
	
	def _declaration(self):
		return 'const %s\t%s' % (self.macro('int'), self.clean_symbol())
	
	def const_declaration(self):
		return self._declaration() + '(1);'
	
	def extern_const_declaration(self):
		return 'extern ' + self._declaration() + ';'

	def type_decl(self, valuetype=None):
		name_parts = self.group.quantities[0].split(' ')
		name = ''.join(name_part.capitalize() for name_part in name_parts)
		
		result = name + '_' + self.clean_symbol()
		if valuetype is not None:
			result += '<' + valuetype + '>'
		return result
	
	def clean_symbol(self):
		return self.symbol.replace('μ', 'u').replace('/', '_').replace('²', '2').replace('³', '3')
	
	UNITS = {}


class SIValue(object):
	def __init__(self, *unit_powers):
		self.unit_powers = unit_powers
	
	def fmt(self, valuetype_var):
		powers = itertools.islice(itertools.chain(self.unit_powers, itertools.repeat(0)), 7)
		powers = ', '.join(str(i) for i in powers)
		return '::si::SIValue<%s, ::std::ratio<1>, %s>' % (valuetype_var, powers)
	

class ApplyRatio(object):
	def __init__(self, from_symbol, ratio):
		self.from_symbol = from_symbol
		self.ratio = ratio
	
	def fmt(self, valuetype_var):
		ratio = self.ratio
		if isinstance(ratio, int):
			ratio = ' ::std::ratio<%d>' % ratio
		
		unit = Unit.UNITS[self.from_symbol]
		
		from_ = unit.macro(valuetype_var)
		
		return '%s::apply_ratio<%s>::type' % (from_, ratio)


class TypeOperation(object):
	def __init__(self, symbol1, symbol2):
		self.symbol1 = symbol1
		self.symbol2 = symbol2

	def fmt(self, valuetype):
		type1 = self._get_type(self.symbol1, valuetype)
		type2 = self._get_type(self.symbol2, valuetype)
		return '::si::%s<%s, %s>::type' % (self.operation(), type1, type2)
	
	def _get_type(self, symbol, valuetype):
		if symbol is None:
			return valuetype
		else:
			unit = Unit.UNITS[symbol]
			return unit.macro(valuetype)


class Multiplication(TypeOperation):
	def operation(self):
		return 'multiplication'


class Division(TypeOperation):
	def operation(self):
		return 'division'
	


##### UNITS ####################################################################

Group(True, ['length', 'distance'], 'm',
	Unit('mm', 'millimeter', 'millimeters', ApplyRatio('m', ' ::std::milli')),
	Unit('cm', 'centimeter', 'centimeters', ApplyRatio('m', ' ::std::centi')),
	Unit('m' ,      'meter',      'meters', SIValue(1)),
	Unit('km',  'kilometer',  'kilometers', ApplyRatio('m', ' ::std::kilo')),
)

Group(True, 'time', 's',
	Unit('ns' ,  'nanosecond',  'nanoseconds', ApplyRatio('s', ' ::std::nano')),
	Unit('μs' , 'microsecond', 'microseconds', ApplyRatio('s', ' ::std::micro')),
	Unit('ms' , 'millisecond', 'milliseconds', ApplyRatio('s', ' ::std::milli')),
	Unit('s'  ,      'second',      'seconds', SIValue(0, 0, 1)),
	Unit('min',      'minute',      'minutes', ApplyRatio('s'  , 60)),
	Unit('h'  ,        'hour',        'hours', ApplyRatio('min', 60)),
	Unit('d'  ,         'day',         'days', ApplyRatio('h'  , 24)),
)

Group(True, 'electric current', 'A',
	Unit('A', 'ampere', 'amperes', SIValue(0, 0, 0, 1)),
)

Group(False, 'area', 'm²',
	Unit('cm²', 'square centimeter', 'square centimeters', Multiplication('cm', 'cm')),
	Unit( 'm²', 'square meter'     , 'square meters'     , Multiplication( 'm',  'm')),
	Unit('km²', 'square kilometer' , 'square kilometers' , Multiplication('km', 'km')),
)

Group(False, 'volume', 'm³',
	Unit('m³', 'cubic meter', 'cubic meters', Multiplication('m²', 'm')),
)

Group(False, 'speed', 'm/s',
	Unit('m/s', 'meter per second', 'meters per second', Division('m', 's'))
)

Group(False, 'acceleration', 'm/s²',
	Unit('m/s²', 'meter per second squared', 'meters per second squared', Division('m/s', 's'))
)

Group(False, 'electric charge', 'C',
	Unit('C', 'coulomb', 'coulombs', Multiplication('A', 's'))
)

Group(False, 'frequency', 'Hz',
	Unit('Hz', 'hertz', 'hertz', Division(None, 's'))
)


if __name__ == '__main__':
	main()
