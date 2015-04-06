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
    [WebService(Namespace = "http://tempuri.org/")]
    [WebServiceBinding(ConformsTo = WsiProfiles.BasicProfile1_1)]
    [System.ComponentModel.ToolboxItem(false)]
    // To allow this Web Service to be called from script, using ASP.NET AJAX, uncomment the following line. 
    // [System.Web.Script.Services.ScriptService]
    public class Service1 : System.Web.Services.WebService
    {

        [WebMethod]
        public string HelloWorld()
        {
            return "Hello World";
        }

        [WebMethod]
        public string ReadPSoC() 
        {
            string line = "";

            SerialPort s = new SerialPort("COM3", 9600);

            if (s != null)
            {

                s.Open();
                s.WriteLine("OK");
                line = s.ReadLine();
                s.Close();
                return line;

            } else {
                return "null";
            }

        }

    }
}