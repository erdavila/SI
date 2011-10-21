#!/usr/bin/env python
# -*- encoding: utf-8 -*-

from __future__ import print_function
import itertools
import re


		
def print_lines(f, line):
	# Remove initial newline
	if line and line[0] == '\n': line = line[1:]
	
	m = re.match(r'\t*', line)
	ident = m.group()
	ident_size = len(ident)
	
	# Remove the identation in the first line
	line = line[ident_size:]
	
	# Remove the identation in the remaining lines
	line = line.replace('\n' + ident, '\n')
	
	## Remove identation
	#line = re.sub(r'\n\t{4}', r'\n', line, flags=re.MULTILINE)
	
	# Remove tabs in the end
	line = re.sub(r'\n\t+', '', line)
	
	print(line, file=f)


def generate_defs():
	with file('defs.hpp', 'w') as f:
		print_lines(f,
			'''
				#ifndef SI_DEFS_HPP_
				#define SI_DEFS_HPP_
				
				
				#include "si_value.hpp"
				
				
				/** @file */
			''')
		
		for group in Group.GROUPS:
			print_lines(f,
					'''
						
						
						%s
						//@{
					''' % group.header()
				)
			
			for unit in group.units:
				print_lines(f,
						'''
							/// %s in %s
							#define %s\t%s
						''' % (group.name(), unit.plural, unit.macro('VALUETYPE'), unit.definition)
					)
				
				#print_lines('/// %s in %s' % (group.name(), unit.plural))
				#print_lines('#define %s\t%s' % (unit.macro('VALUETYPE'), unit.definition))
			
			print_lines(f, '//@}')
		
		print_lines(f,
			'''
				
				
				#endif /* SI_DEFS_HPP_ */
			''')
		

def generate_units():
	with file('units.cpp', 'w') as f:
		print_lines(f,
			'''
				#include "units.hpp"
				
				
				namespace si {
				namespace units {
				
			''')
		
		for group in Group.GROUPS:
			print_lines(f, '')
			
			for unit in group.units:
				print_lines(f, 'const %s\t%s(1);' % (unit.macro('int'), unit.clean_symbol()))
		
		print_lines(f,
			'''
				
				
				} /* namespace si::units */
				} /* namespace si */
			''')
		

def generate_units_header():
	with file('units.hpp', 'w') as f:
		print_lines(f,
			'''
				#ifndef SI_UNITS_HPP_
				#define SI_UNITS_HPP_
				
				
				#include "defs.hpp"
				
				
				namespace si {
				/// Pre-defined unit values that help to create other values.
				namespace units {
			''')
		
		for group in Group.GROUPS:
			print_lines(f,
					'''
						
						
						%s
						//@{
					''' % group.header()
				)
			
			for unit in group.units:
				print_lines(f,
						'''
							/// 1%s (1 %s)
							extern const %s\t%s;
						''' % (unit.symbol, unit.singular, unit.macro('int'), unit.clean_symbol())
					)
						
			print_lines(f, '//@}')
		
		print_lines(f,
			'''
				
				
				} /* namespace si::units */
				} /* namespace si */
				
				
				#endif /* SI_UNITS_HPP_ */
			''')
		

def main():
	generate_defs()
	generate_units()
	generate_units_header()



class Group(object):
	def __init__(self, is_base_unit, quantity, symbol, *units):
		self.is_base_unit = is_base_unit
		self.quantity = quantity
		self.symbol = symbol
		self.units = units
		for unit in units:
			unit.group = self
		Group.GROUPS.append(self)
	
	def name(self):
		if isinstance(self.quantity, str):
			name = self.quantity
		else:
			name = ', '.join(self.quantity)
		return name.capitalize()
	
	def header(self):
		unit_name = Unit.UNITS[self.symbol].singular
		
		return '\n'.join([
				'/** @name %s' % self.name(),
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
		quantity = self.group.quantity
		if not isinstance(quantity, str):
			quantity = quantity[0]
		quantity = quantity.upper().replace(' ', '')
		
		return 'SI_%s_%s(%s)' % (quantity, self.clean_symbol(), argument)
	
	def clean_symbol(self):
		return self.symbol.replace('μ', 'u').replace('/', '_').replace('²', '2').replace('³', '3')
	
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
		ratio = self.ratio
		if isinstance(ratio, int):
			ratio = ' ::std::ratio<%d>' % ratio
		
		return '%s::apply_ratio<%s>::type' % (Unit.UNITS[self.from_symbol].macro('VALUETYPE'), ratio)


class TypeOperation(object):
	def __init__(self, symbol1, symbol2):
		self.symbol1 = symbol1
		self.symbol2 = symbol2

	def __str__(self):
		macro1 = 'VALUETYPE' if self.symbol1 is None else Unit.UNITS[self.symbol1].macro('VALUETYPE')
		macro2 = 'VALUETYPE' if self.symbol2 is None else Unit.UNITS[self.symbol2].macro('VALUETYPE')
		return '::si::%s<%s, %s>::type' % (self.operation(), macro1, macro2)


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
