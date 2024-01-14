import xmltodict

with open('JMdict_e.xml', 'r') as f:
	data = f.read()

parsed = xmltodict.parse(data)

lines = []

for entry in parsed['JMdict']['entry']:
  kanji = []
  k_ele_list = entry.get('k_ele', {})
  if isinstance(k_ele_list, dict):
    k_ele_list = [k_ele_list]
  if isinstance(k_ele_list, list):
    for k_ele in k_ele_list:
      keb = k_ele.get('keb', {})
      if isinstance(keb, dict):
        kanji.append(keb.get('#text', ''))
      else:
        kanji.append(keb)


  furigana = []
  r_ele_list = entry.get('r_ele', [])
  if isinstance(r_ele_list, dict):
    r_ele_list = [r_ele_list]
  if isinstance(r_ele_list, list):
    for r_ele in r_ele_list:
      reb = r_ele.get('reb', {})
      if isinstance(reb, dict):
        furigana.append(reb.get('#text', ''))
      else:
        furigana.append(reb)

  glosses = []
  sense_list = entry.get('sense', [])
  if isinstance(sense_list, dict):
    sense_list = [sense_list]
  if isinstance(sense_list, list):
    for sense in sense_list:
      gloss_list = sense.get('gloss', [])
      if isinstance(gloss_list, list):
        for gloss in gloss_list:
          if isinstance(gloss, dict):
            glosses.append(gloss.get('#text', ''))
          else:
            glosses.append(gloss)
      else:
        glosses.append(gloss_list.get('#text', ''))

  lines.append('|'.join(['§'.join(kanji), '§'.join(furigana), '§'.join(glosses)]))

smol = '\n'.join(lines)

with open('JMdict_smol.txt', 'w') as filetowrite:
    filetowrite.write(smol)

print(smol)
