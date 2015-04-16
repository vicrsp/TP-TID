<%@ Page Language="C#" AutoEventWireup="true" CodeBehind="ClienteTP.aspx.cs" Inherits="ClienteWS_TP.ClienteTP" %>

<!DOCTYPE html>

<html xmlns="http://www.w3.org/1999/xhtml">
<head runat="server">
    <title></title>
</head>
<body>
    <form id="form1" runat="server">
        
        <table style="font-family: Arial">
            <tr>
                <td>
                    <asp:Button ID="btnRead" runat="server" Text="Read from PSoC" OnClick ="btnRead_Click"/>
                </td>
                <td>
                    <asp:Label ID="lblResult" runat="server"></asp:Label>
                </td>
            </tr>
            <tr>
                <td>
                    <asp:GridView ID="gvReadings" runat="server">
                </asp:GridView>
                </td>
            </tr>
        </table>
    </form>
</body>
</html>
