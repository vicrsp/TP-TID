using System;
using System.Collections.Generic;
using System.Linq;
using System.Web;
using System.Web.Services;
using System.IO.Ports;

namespace WS_TP
{
    /// <summary>
    /// Summary description for Service1
    /// </summary>
    [WebService(Namespace = "http://ufmg.br/TPTID/")]
    [WebServiceBinding(ConformsTo = WsiProfiles.BasicProfile1_1)]
    [System.ComponentModel.ToolboxItem(false)]
    // To allow this Web Service to be called from script, using ASP.NET AJAX, uncomment the following line. 
    // [System.Web.Script.Services.ScriptService]
    public class Service1 : System.Web.Services.WebService
    {

        [WebMethod(EnableSession=true)]
        public string ReadPSoC()
        {
            string line = "";
            List<string> temperatures;

            try
            {
                SerialPort s = new SerialPort("COM3", 9600);
            
            if (Session["READINGS"] == null)
            {
                temperatures = new List<string>();
            }
            else 
            {
                temperatures = (List<string>)Session["READINGS"];
            }

            
            if (s != null)
            {

                s.Open();
                s.WriteLine("OK");
                line = s.ReadLine();
                s.Close();
                System.Threading.Thread.Sleep(1000);

                line = line + "°C  --  "  + DateTime.Now.ToLongTimeString();
                temperatures.Add(line);
                Session["READINGS"] = temperatures;

                return line;
                    
            }
            else
            {
                temperatures.Add("Failed");
                Session["READINGS"] = temperatures;
                return "Failed";
            }
            }
            catch (Exception ex) { return "Error opening port"; }


        }
        [WebMethod(EnableSession = true)]
        public List<string> getReadings() 
        {
            if (Session["READINGS"] == null)
            {
                List<string> temperatures = new List<string>();
                temperatures.Add("You have not read anything yet");
                return temperatures;
            }
            else 
            {
                return (List<string>)Session["READINGS"];
            }

        
        
        }

    }
}