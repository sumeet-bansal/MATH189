import pandas as pd

train = pd.read_csv('train.csv', header=0)
train = train[train['comment_text'].str.contains('UTC')]

df = {}
df['toxic'] = []
df['severe_toxic'] = []
df['obscene'] = []
df['threat'] = []
df['insult'] = []
df['identity_hate'] = []
for i, row in train.iterrows():
	ct = row['comment_text']
	if ' (UTC)' not in ct:
		continue
	eit = ct.index(' (UTC)')
	if ct[eit-4:eit-2] != '20':
		continue
	year = int(ct[eit-4:eit])
	for key in df:
		if row[key] == 1:
			df[key].append(year)

minyear = 3000
maxyear = -1
for key in df:
	minyear = min(min(df[key]), minyear)
	maxyear = max(max(df[key]), maxyear)
for key in df:
	counts = {}
	for y in range(minyear, maxyear + 1):
		counts[y] = df[key].count(y)
	df[key] = counts

with open('timestamped.csv', 'w') as csv:
	csv.write('classification, %s\n' % str(list(df['toxic'].keys()))[1:-1])
	for key in df:
		csv.write("%s, %s\n" % (key, str(list(df[key].values()))[1:-1]))
