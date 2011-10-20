#!/usr/bin/env python
# -*- encoding: utf-8 -*-

from __future__ import print_function
from timeit import itertools


def generate_defs():
	with file('defs.hpp', 'w') as f:
		def print_line(line):
			print(line, file=f)
		
		print_line('#ifndef SI_DEFS_HPP_')
		print_line('#define SI_DEFS_HPP_')
		print_line('')
		print_line('')
		print_line('#include "si_value.hpp"')
		print_line('')
		print_line('')
		print_line('/** @file */')
		
		for group in Group.GROUPS:
			print_line('')
			print_line('')
			print_line(group.header())
			print_line('//@{')
			
			for unit in group.units:
				print_line('/// %s' % unit.measure)
				print_line('#define %s(VALUETYPE)\t%s' % (unit.macro(), unit.definition))
			
			print_line('//@}')
		
		print_line('')
		print_line('')
		print_line('#endif /* SI_DEFS_HPP_ */')
		

def generate_units_cpp():
	with file('units.cpp', 'w') as f:
		def print_line(line):
			print(line, file=f)
		
		print_line('#include "units.hpp"')
		print_line('')
		print_line('')
		print_line('namespace si {')
		print_line('namespace units {')
		
		for group in Group.GROUPS:
			print_line('')
			print_line('')
			
			for unit in group.units:
				print_line('const %s(int)\t%s(1);' % (unit.macro(), unit.clean_symbol()))
		
		print_line('')
		print_line('')
		print_line('} /* namespace si::units */')
		print_line('} /* namespace si */')
		

def generate_units_hpp():
	with file('units.hpp', 'w') as f:
		def print_line(line):
			print(line, file=f)

		print_line('#ifndef SI_UNITS_HPP_')
		print_line('#define SI_UNITS_HPP_')
		print_line('')
		print_line('')
		print_line('#include "defs.hpp"')
		print_line('')
		print_line('')
		print_line('namespace si {')
		print_line('/// Pre-defined unit values that help to create other values.')
		print_line('namespace units {')
		
		for group in Group.GROUPS:
			print_line('')
			print_line('')
			print_line(group.header())
			print_line('//@{')
			
			for unit in group.units:
				print_line('/// 1%s (1%s)' % (unit.symbol, unit.singular))
				print_line('extern const %s(int)\t%s;' % (unit.macro(), unit.clean_symbol()))
			
			print_line('//@}')
		
		print_line('')
		print_line('')
		print_line('} /* namespace si::units */')
		print_line('} /* namespace si */')
		print_line('')
		print_line('')
		print_line('#endif /* SI_UNITS_HPP_ */')
		

def main():
	generate_defs()
	generate_units_cpp()
	generate_units_hpp()



class Group(object):
	def __init__(self, is_base_unit, quantity, unit_name, symbol, *units):
		self.is_base_unit = is_base_unit
		self.quantity = quantity
		self.unit_name = unit_name
		self.symbol = symbol
		self.units = units
		for unit in units:
			unit.group = self
		Group.GROUPS.append(self)
	
	def group_name(self):
		if isinstance(self.quantity, str):
			group_name = self.quantity
		else:
			group_name = ', '.join(self.quantity)
		return group_name.capitalize()
	
	def header(self):
		return '\n'.join([
				'/** @name %s' % self.group_name(),
				' * %s unit: %s<br>' % ('Base' if self.is_base_unit else 'Derived', self.unit_name),
				' * Symbol: %s' % self.symbol,
				' */'
			])
	
	GROUPS = []


class Unit(object):
	def __init__(self, symbol, measure, singular, definition):
		self.symbol = symbol
		self.measure = measure
		self.singular = singular
		self.definition = definition
		if symbol in Unit.UNITS:
			raise Exception(symbol)
		Unit.UNITS[symbol] = self
	
	def macro(self):
		quantity = self.group.quantity
		if not isinstance(quantity, str):
			quantity = quantity[0]
		quantity = quantity.upper()
		
		return 'SI_%s_%s' % (quantity, self.clean_symbol())
	
	def clean_symbol(self):
		return self.symbol.replace('μ', 'u').replace('/', '_').replace('²', '2')
	
	UNITS = {}


class SIValue(object):
	def __init__(self, *unit_powers):
		self.unit_powers = unit_powers
	
	def __str__(self):
		return '::si::SIValue<VALUETYPE, ::std::ratio<1>, %s>' % ', '.join(str(i) for i in itertools.islice(itertools.chain(self.unit_powers, itertools.repeat(0)), 7))


class ApplyRatio(object):
	def __init__(self, from_symbol, ratio):
		self.from_symbol = from_symbol
		self.ratio = ratio
	
	def __str__(self):
		return '%s(VALUETYPE)::apply_ratio<%s>::type' % (Unit.UNITS[self.from_symbol].macro(), self.ratio)


class Division(object):
	def __init__(self, symbol1, symbol2):
		self.symbol1 = symbol1
		self.symbol2 = symbol2
	
	def __str__(self):
		macro1 = Unit.UNITS[self.symbol1].macro()
		macro2 = Unit.UNITS[self.symbol2].macro()
		return '::si::division<%s(VALUETYPE), %s(VALUETYPE)>::type' % (macro1, macro2)


##### UNITS ####################################################################

Group(True, ['length', 'distance'], 'meter', 'm',
	Unit('mm', 'Length in millimeters', 'millimeter', ApplyRatio('m', ' ::std::milli')),
	Unit('cm', 'Length in centimeters', 'centimeter', ApplyRatio('m', ' ::std::centi')),
	Unit('m' , 'Length in meters'     ,      'meter', SIValue(1)),
	Unit('km', 'Length in kilometers' ,  'kilometer', ApplyRatio('m', ' ::std::kilo')),
)

Group(True, 'time', 'second', 's',
	Unit('ns' , 'Time in nanoseconds' ,  'nanosecond', ApplyRatio('s', ' ::std::nano')),
	Unit('μs' , 'Time in microseconds', 'microsecond', ApplyRatio('s', ' ::std::micro')),
	Unit('ms' , 'Time in milliseconds', 'millisecond', ApplyRatio('s', ' ::std::milli')),
	Unit('s'  , 'Time in seconds'     ,      'second', SIValue(0, 0, 1)),
	Unit('min', 'Time in minutes'     ,      'minute', ApplyRatio('s'  , ' ::std::ratio<%d>' % 60)),
	Unit('h'  , 'Time in hours'       ,        'hour', ApplyRatio('min', ' ::std::ratio<%d>' % 60)),
	Unit('d'  , 'Time in days'        ,         'day', ApplyRatio('h'  , ' ::std::ratio<%d>' % 24)),
)

Group(False, 'speed', 'meters per second', 'm/s',
	Unit('m/s', 'Speed in meters per second', 'meter per second', Division('m', 's'))
)

Group(False, 'acceleration', 'meters per second squared', 'm/s²',
	Unit('m/s²', 'Acceleration in meters per second squared', 'meter per second squared', Division('m/s', 's'))
)


if __name__ == '__main__':
	main()
