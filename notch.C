#include "TGraph.h"
#include "TString.h"
#include "TLegend.h"
#include "TROOT.h"
#include "TCanvas.h"
#include "TH1.h"
#include "TF1.h"
#include "TMath.h"

TString graphname = "notch_R";
TString datfile = ".dat";
TGraph *g[4];
/*Double_t f(Double_t *x, Double_t *par)
{
    // xx, frequenza
    //  par0 resistenza
    //  par1 induttanza
    //  par2 capacità

    Float_t xx = x[0];
    Double_t val = 0.5 * TMath::Sqrt(((par[0] * par[0]) * ((xx * xx * par[1] * par[2]) - 1) * ((xx * xx * par[1] * par[2]) - 1)) / (((par[0] * par[0]) * ((xx * xx * par[1] * par[2]) - 1) * ((xx * xx * par[1] * par[2]) - 1)) + (xx * xx) * par[1] * par[1]));
    return val;
}*/

void create()
{
    for (size_t i = 1; i < 4; i++)
    {
        g[i] = new TGraph(graphname + i + datfile);
        g[i]->GetXaxis()->SetTitle("Frequenza(Hz)");
        g[i]->GetYaxis()->SetTitle("Tensione(V)");
        g[i]->SetLineColor(kBlue);
    }
}

void fit()
{
    create();
    TCanvas *Cf = new TCanvas("Cf", "Fit");
    // TF1 *H = new TF1("H", "0.5 * TMath::Sqrt( (([0]^2) * ((x^2 * [1]*[2])-1)^2 ) / ( ( ([0]^2 ) * (( x^2 *[1]*[2])-1 )^2) + (x^2) * [1]^2 ) )", 0, 80000);
    TF1 *H = new TF1("H", "0.5 * TMath::Sqrt( (([0]^2) * (((x*2*3.14) ^2 * [1]*[2])-1)^2 )/ ( ( ([0]^2 ) * (((x * 2*3.14) ^2*[1]*[2])-1 )^2) + ((x *2*3.14) ^2) * [1]^2 ) )", 0, 18000);
    H->SetParameter(0, 6800);
    H->SetParameter(1, 48.8e-3);
    H->SetParameter(2, 15.3e-9);
    g[3]->Fit("H");
    g[3]->SetMarkerStyle(6);
    TF1 *fit = g[3]->GetFunction("H");
    H->Draw();
    g[3]->Draw();
}

void separated()
{
    create();
    TCanvas *C1 = new TCanvas("C1", "Nocth Filter Separated");
    TCanvas *C[4];
    C1->Divide(1, 3);
    TString graph = "Filtro Notch con R";
    TString filename = "Notch_R";
    TString pdffile = ".pdf";
    TLegend *leg[4];

    for (size_t i = 1; i < 4; i++)
    {
        // SingleCancvas
        C1->cd(i);
        g[i]->SetMarkerStyle(6);
        g[i]->SetTitle(graph + i);
        g[i]->Draw();
        // MultiCanvas
        C[i] = new TCanvas(graph + i, "Notch Single");
        g[i]->Draw();
        leg[i] = new TLegend(0.1, 0.7, 0.3, 0.9);
        leg[i]->AddEntry(g[i], "Tensione");
        leg[i]->Draw("Voltage");
        C[i]->Print(filename + i + pdffile);
    }

    C1->Print("Notch_Separated.pdf");
}

void comparison()
{
    create();
    TCanvas *C2 = new TCanvas("C2", "Nocth Filters Comparison");
    g[1]->SetTitle("Confronto tra filtri Notch con diverse resistenze");
    g[2]->SetLineColor(kRed);
    g[3]->SetLineColor(kGreen);
    g[1]->Draw();
    g[2]->Draw("SAME");
    g[3]->Draw("SAME");
    TLegend *leg2 = new TLegend(0.1, 0.7, 0.3, 0.9, "Tensione");
    TString legR = "R=R";
    for (size_t i = 1; i < 4; i++)
    {
        leg2->AddEntry(g[i], legR + i);
    }

    leg2->Draw("SAME");
    C2->Print("Notch_Comparison.pdf");
}