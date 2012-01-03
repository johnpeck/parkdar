""" sch2ps.py
    Convert schematic files to postscript. """
import json, os, sys
projfile = '../../projdefs.json'
printscript = 'print.scm'
printdir = './printable'

""" Load the project definition file if it exists """
try:
    sys.stdout.write ('Opening the project definition file at ' +
                       projfile + '...')
    fin = open(projfile, "ro")
    sys.stdout.write ('done.\n')
except IOError as e:
    print ('Could not open the project definition file ' + projfile + '.')
    sys.exit()
projdefs = json.loads(fin.read())

""" Check to see if the print guile script is in this directory """
try:
    open(printscript,'ro')
except IOError as e:
    print ('Could not open the guile script ' + printscript + '.')
    sys.exit()
    
""" Create the print directory if necessary """
if (not os.access(printdir,os.F_OK)):
    os.mkdir(printdir)
    print ('* Created ' + printdir)
else:
    print('* ' + printdir + " already exists.  I'll overwrite it.")


""" Convert each schematic page to postscript """
pslist = []
for page in projdefs['schematic_pages']:
    try:
        open(page,'ro')
        outfile =  printdir + '/' + page.split('.')[0] + '.ps'
        sys.stdout.write('Converting ' + page + ' to ' + outfile + '...')
        os.popen('gschem -p -o ' + outfile + ' -s ' + printscript +
                 ' ' + page)
        sys.stdout.write('done.\n')
        pslist.append(outfile) # Make a list of ps outputs for joining
    except IOError as e:
        print ('Could not open the schematic page ' + page + '.')
        sys.exit()
        
""" Join postscript files """
allpsname = (printdir + '/' + projdefs['project_name'] + '_schematics.ps')
try:
    os.popen('gs -sDEVICE=pswrite -dNOPAUSE -dBATCH -dSAFER -sOutputFile=' + 
            allpsname + ' ' + ' '.join(pslist)) 
except:
    sys.exit()

""" Convert postscript files to pdf if they exist """
for page in projdefs['schematic_pages']:
    psfile =  printdir + '/' + page.split('.')[0] + '.ps'
    pdfout =  printdir + '/' + page.split('.')[0] + '.pdf'
    if (os.access(psfile,os.F_OK)):
        sys.stdout.write('Converting ' + page + ' to pdf...')
        os.popen('ps2pdfwr ' + psfile + ' ' + pdfout)
        sys.stdout.write('done.\n')


fin.close()
