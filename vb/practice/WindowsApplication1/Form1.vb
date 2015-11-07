Public Class Form1


    Private Sub Form1_Load(sender As Object, e As EventArgs) Handles MyBase.Load
        AddHandler Button1.Click, AddressOf Me.Button_Click
    End Sub

    Private Sub Button_Click(sender As Object, e As EventArgs)
        MsgBox("Clicked!", , )
    End Sub
End Class
