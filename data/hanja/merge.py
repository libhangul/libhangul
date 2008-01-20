#!/usr/bin/env python

import sys

header = []

# load freq table
freq_table = {}

def load_frequency(filename):
    try:
	src = open(filename, 'r')
	for i in src.readlines():
	    list = i.strip().decode('utf-8').split(u':')
	    key = list[0]
	    value = float(list[1])
	    if freq_table.has_key(key):
		freq_table[key] = max(freq_table[key], value)
	    else:
		freq_table[key] = value

	src.close()
    except:
	return

def get_frequency(key):
    if freq_table.has_key(key):
	return freq_table[key]
    else:
	return 0

load_frequency('freq-hanja.txt')
load_frequency('freq-hanjaeo.txt')

table = {}
for file in sys.argv[1:]:
    try:
	src = open(file, 'r')

	for line in src.readlines():
	    if line.startswith('#'):
		header += line;
		continue;

	    line = line.strip().decode('utf-8').split(u':')
	    if len(line) < 3:
		continue

	    key = line[0]
	    value = line[1]
	    comment = line[2]
	    freq = get_frequency(value)

	    if table.has_key(key):
		isDuplicate = False
		# check duplicate
		for i in table[key]:
		    if i['value'] == value and i['comment'] == comment:
			isDuplicate = True
			sys.stderr.write('%s:%s is duplicate\n' % (key.encode('utf-8'), value('utf-8')))
			continue
		    elif i['value'] == value and i['comment'] != comment:
			sys.stderr.write('%s:%s has different comments\n' % (key.encode('utf-8'), value('utf-8')))
			sys.stderr.write('\t%s\n' % (i['comment'].encode('utf-8')))
			sys.stderr.write('\t%s\n' % (comment.encode('utf-8')))
			isDuplicate = True
			continue

		if not isDuplicate:
		    table[key].append({ 'key' : key, 'value': value, 'freq': freq, 'comment': comment })
	    else:
		table[key] = [ { 'key': key, 'value': value, 'freq': freq, 'comment': comment } ]

	src.close()

    except:
	continue

keys = table.keys()
keys.sort()

for i in header:
    sys.stdout.write(i)
sys.stdout.write('\n')

def cmp(x, y):
    if x['freq'] > y['freq']:
	return -1
    elif x['freq'] < y['freq']:
	return 1
    else:
	return 0

for key in keys:
    list = table[key]
    key = key.encode('utf-8')

    list.sort(cmp)
    for i in list:
	value   = i['value'].encode('utf-8')
	freq    = i['freq']
	comment = i['comment'].encode('utf-8')

	sys.stdout.write('%s:%s:%s\n' % (key, value, comment))
