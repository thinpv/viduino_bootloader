# sheet-viduino-id-list@viduino.iam.gserviceaccount.com

# importing the required libraries
import pandas as pd
import gspread
from oauth2client.service_account import ServiceAccountCredentials
import sys
import os
from datetime import datetime

sys.argv.pop(0)
inName = sys.argv.pop(0)

ss_cred_path = os.path.dirname(os.path.realpath(__file__)) + '/viduino-dc697049ddc0.json' # Your path to the json credential file
scope = ['https://spreadsheets.google.com/feeds','https://www.googleapis.com/auth/drive'] # define the scope
creds = ServiceAccountCredentials.from_json_keyfile_name(ss_cred_path, scope) # add credentials to the account
gc = gspread.authorize(creds) # authorize the clientsheet 

# Get existing trend data from Sheet
spreadsheet_id = "1WD232mF1odXyaAtB7GLJaQrKbeZuvuXCdUtHMPaYQvc"
wks = gc.open_by_key(spreadsheet_id)
worksheet = wks.worksheet('ids')
cell = worksheet.find(inName)
if(cell == None):
	now = datetime.now()
	values_list = worksheet.col_values(1)
	worksheet.update_cell(len(values_list) + 1, 1, len(values_list))
	worksheet.update_cell(len(values_list) + 1, 2, now.strftime("%d/%m/%Y %H:%M:%S"))
	worksheet.update_cell(len(values_list) + 1, 3, inName)
else:
	print("Found something at R%s-C%s" % (cell.row, cell.col))