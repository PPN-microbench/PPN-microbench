from elements import *

import traceback
import os

if __name__ == "__main__":
    report = Report("../tmp/out.json")

    if not os.path.exists("./out"):
        os.mkdir("./out")

    with open("out/haha.html", "w") as f:
        f.write(report.to_html())

    try:
        from weasyprint import HTML, CSS
        try:
            css = CSS(string="""
                @page {size: A4; margin: 1cm;} 
                th, td {border: 1px solid black;}
                """)
            HTML("out/haha.html").write_pdf("out/haha.pdf", stylesheets=[css])
        except Exception as e:
            traceback.print_exc()
    except:
        print("No way to export report to pdf. Run `pip install weasyprint`.")
