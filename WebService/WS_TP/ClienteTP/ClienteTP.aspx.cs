using System;
using System.Collections.Generic;
using System.Linq;
using System.Web;
using System.Web.UI;
using System.Web.UI.WebControls;

namespace ClienteWS_TP
{
    public partial class ClienteTP : System.Web.UI.Page
    {
        protected void Page_Load(object sender, EventArgs e)
        {

        }
        protected void btnRead_Click(object sender, EventArgs e)
        {

            ReadPSoCService.Service1SoapClient client =
                new ReadPSoCService.Service1SoapClient();

            
            string result = client.ReadPSoC();
            lblResult.Text = result;

            gvReadings.DataSource = client.getReadings();
            gvReadings.DataBind();

            gvReadings.HeaderRow.Cells[0].Text = "Recent Temperature Readings";
        }
      

    }
}