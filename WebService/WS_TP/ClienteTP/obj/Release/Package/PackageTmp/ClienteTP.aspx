<%@ Page Language="C#" AutoEventWireup="true" CodeBehind="ClienteTP.aspx.cs" Inherits="ClienteWS_TP.ClienteTP" %>

<!DOCTYPE html>

<html xmlns="http://www.w3.org/1999/xhtml">
<head runat="server">
<title> Trabalho prático - Técnicas de Integração de Dados em Automação </title>
</head>
<body>
    <form id="form1" runat="server">
       <h1> Web Client para consumir o Web Service </h1>
       <h2> Pressione o botão para ler dados do PSoC </h2>
       <h2> O último valor lido será mostrado à direita do botão.</h2> 
       <h2> A tabela abaixo contém o histórico de leituras.</h2>
        <table style="font-family: Arial">
            <tr>
                <td>
                    <asp:Button ID="btnRead" runat="server" Text="Ler temperatura" OnClick ="btnRead_Click"/>
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
