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

# Base units #

Group(True, ['length', 'distance'], 'm',
	Unit('nm',  'nanometer',  'nanometers', ApplyRatio('m', ' ::std::nano')),
	Unit('μm', 'micrometer', 'micrometers', ApplyRatio('m', ' ::std::micro')),
	Unit('mm', 'millimeter', 'millimeters', ApplyRatio('m', ' ::std::milli')),
	Unit('cm', 'centimeter', 'centimeters', ApplyRatio('m', ' ::std::centi')),
	Unit('m' ,      'meter',      'meters', SIValue(1)),
	Unit('km',  'kilometer',  'kilometers', ApplyRatio('m', ' ::std::kilo')),
)

Group(True, 'mass', 'g',
	Unit('pg',  'picogram',  'picograms', ApplyRatio('g', ' ::std::pico')),
	Unit('ng',  'nanogram',  'nanograms', ApplyRatio('g', ' ::std::nano')),
	Unit('μg', 'microgram', 'micrograms', ApplyRatio('g', ' ::std::micro')),
	Unit('mg', 'milligram', 'milligrams', ApplyRatio('g', ' ::std::milli')),
	Unit('cg', 'centigram', 'centigrams', ApplyRatio('g', ' ::std::centi')),
	Unit( 'g',      'gram',      'grams', SIValue(0, 1)),
	Unit('kg',  'kilogram',  'kilograms', ApplyRatio('g', ' ::std::kilo')),
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
	Unit('mA', 'milliampere', 'milliamperes', ApplyRatio('A', ' ::std::milli')),
	Unit( 'A',      'ampere',      'amperes', SIValue(0, 0, 0, 1)),
)

Group(True, 'temperature', 'K',
	Unit('K', 'kelvin', 'kelvins', SIValue(0, 0, 0, 0, 1)),
)

Group(True, 'luminous intensity', 'cd',
	Unit('cd', 'candela', 'candelas', SIValue(0, 0, 0, 0, 0, 1)),
)

Group(True, 'amount of substance', 'mol',
	Unit('mol', 'mole', 'moles', SIValue(0, 0, 0, 0, 0, 0, 1)),
)


# Derived units #

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

Group(False, ['force', 'weight'], 'N',
	Unit('N', 'newton', 'newtons', Multiplication('kg', 'm/s²'))
)

Group(False, ['pressure', 'stress'], 'Pa',
	Unit( 'Pa',      'pascal',      'pascals', Division('N', 'm²')),
	Unit('hPa', 'hectopascal', 'hectopascals', ApplyRatio('Pa', ' ::std::hecto')),
	Unit('kPa',  'kilopascal',  'kilopascals', ApplyRatio('Pa', ' ::std::kilo')),
	Unit('MPa',  'megapascal',  'megapascals', ApplyRatio('Pa', ' ::std::mega')),
	Unit('GPa',  'gigapascal',  'gigapascals', ApplyRatio('Pa', ' ::std::giga')),
)

Group(False, ['energy', 'work', 'heat'], 'J',
	Unit('pJ',  'picojoule',  'picojoules', ApplyRatio('J', ' ::std::pico')),
	Unit('nJ',  'nanojoule',  'nanojoules', ApplyRatio('J', ' ::std::nano')),
	Unit('μJ', 'microjoule', 'microjoules', ApplyRatio('J', ' ::std::micro')),
	Unit('mJ', 'millijoule', 'millijoules', ApplyRatio('J', ' ::std::milli')),
	Unit( 'J',      'joule',      'joules', Multiplication('N', 'm')),
	Unit('kJ',  'kilojoule',  'kilojoules', ApplyRatio('J', ' ::std::kilo')),
	Unit('MJ',  'megajoule',  'megajoules', ApplyRatio('J', ' ::std::mega')),
	Unit('GJ',  'gigajoule',  'gigajoules', ApplyRatio('J', ' ::std::giga')),
	Unit('TJ',  'terajoule',  'terajoules', ApplyRatio('J', ' ::std::tera')),
)

Group(False, 'power', 'W',
	Unit('pW',  'picowatt',  'picowatts', ApplyRatio('W', ' ::std::pico')),
	Unit('nW',  'nanowatt',  'nanowatts', ApplyRatio('W', ' ::std::nano')),
	Unit('μW', 'microwatt', 'microwatts', ApplyRatio('W', ' ::std::micro')),
	Unit('mW', 'milliwatt', 'milliwatts', ApplyRatio('W', ' ::std::milli')),
	Unit( 'W',      'watt',      'watts', Division('J', 's')),
	Unit('kW',  'kilowatt',  'kilowatts', ApplyRatio('W', ' ::std::kilo')),
	Unit('MW',  'megawatt',  'megawatts', ApplyRatio('W', ' ::std::mega')),
	Unit('GW',  'gigawatt',  'gigawatts', ApplyRatio('W', ' ::std::giga')),
	Unit('TW',  'terawatt',  'terawatts', ApplyRatio('W', ' ::std::tera')),
)

Group(False, 'electric charge', 'C',
	Unit('pC',  'picocoulomb',  'picocoulombs', ApplyRatio('C', ' ::std::pico')),
	Unit('nC',  'nanocoulomb',  'nanocoulombs', ApplyRatio('C', ' ::std::nano')),
	Unit('μC', 'microcoulomb', 'microcoulombs', ApplyRatio('C', ' ::std::micro')),
	Unit('mC', 'millicoulomb', 'millicoulombs', ApplyRatio('C', ' ::std::milli')),
	Unit( 'C',      'coulomb',      'coulombs', Multiplication('A', 's')),
)

Group(False, ['voltage', 'electrical potential difference'], 'V',
	Unit('mV', 'millivolt', 'millivolts', ApplyRatio('V', ' ::std::milli')),
	Unit( 'V',      'volt',      'volts', Division('W', 'A')),
	Unit('kV',  'kilovolt',  'kilovolts', ApplyRatio('V', ' ::std::kilo')),
	Unit('MV',  'megavolt',  'megavolts', ApplyRatio('V', ' ::std::mega')),
)

Group(False, 'electric capacitance', 'F',
	Unit('pF',  'picofarad',  'picofarads', ApplyRatio('F', ' ::std::pico')),
	Unit('nF',  'nanofarad',  'nanofarads', ApplyRatio('F', ' ::std::nano')),
	Unit('μF', 'microfarad', 'microfarads', ApplyRatio('F', ' ::std::micro')),
	Unit( 'F',      'farad',      'farads', Division('C', 'V')),
)

Group(False, 'frequency', 'Hz',
	Unit('Hz', 'hertz', 'hertz', Division(None, 's')),
)


if __name__ == '__main__':
	main()
