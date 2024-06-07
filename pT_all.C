#include <iomanip>

void pT_all() {

    TString file_path_Dp = "Dp/GenDpMC_output.root";
    TString file_path_Dst = "Dst/GenDstMC_output.root";
    TString file_path_D0 = "D0/GenD0MC_output.root";
    TString output = "pT_spectra.root";

    TFile *file_Dst = new TFile(file_path_Dst, "READ");
    TFile *file_Dp = new TFile(file_path_Dp, "READ");
    TFile *file_D0 = new TFile(file_path_D0, "READ");
    TFile *file_output = new TFile(output, "RECREATE");

    TTree *Dst_tree = (TTree*)file_Dst->Get("MCDecayTree");
    TTree *Dp_tree = (TTree*)file_Dp->Get("MCDecayTree");
    TTree *D0_tree = (TTree*)file_D0->Get("MCDecayTree");
    
    int entries_Dst = Dst_tree->GetEntries();
    std::cout << "Number of entries in Dst tree: " << entries_Dst << std::endl;
    int entries_Dp = Dp_tree->GetEntries();
    std::cout << "Number of entries in Dp tree: " << entries_Dp << std::endl;
    int entries_D0 = D0_tree->GetEntries();
    std::cout << "Number of entries in D0 tree: " << entries_D0 << std::endl;

    double min_y = 0.0;
    double max_y = 1.0;

    TCut cuts1 = Form("Dstar_TRUEY >= %.1f && Dstar_TRUEY < %.1f", min_y, max_y); 
    TCut cuts2 = Form("Dp_TRUEY >= %.1f && Dp_TRUEY < %.1f", min_y, max_y);
    TCut cuts3 = Form("D0_TRUEY >= %.1f && D0_TRUEY < %.1f", min_y, max_y);

    int nn = 300;
    int pmax = 8;
    TH1D* hist_Dst = new TH1D("pT_Dst", "Dstar_TRUEPT", nn, 0, pmax);
    TH1D* hist_Dp = new TH1D("pT_Dp", "Dp_TRUEPT", nn, 0, pmax);
    TH1D* hist_D0 = new TH1D("pT_D0", "D0_TRUEPT", nn, 0, pmax);

    Dst_tree->Draw("Dstar_TRUEPT / 1000 >> pT_Dst", cuts1);
    Dp_tree->Draw("Dp_TRUEPT / 1000 >> pT_Dp", cuts2);
    D0_tree->Draw("D0_TRUEPT / 1000 >> pT_D0", cuts3);

    hist_Dst->Scale(1/hist_Dst->GetEntries());
    hist_Dp->Scale(1/hist_Dp->GetEntries());
    hist_D0->Scale(1/hist_D0->GetEntries());

    hist_Dst->Write();
    hist_Dp->Write();
    hist_D0->Write();

    auto legend = new TLegend(0.67, 0.6, 0.89, 0.85);
    TCanvas *c1 = new TCanvas("c1", Form("pT Spectrum (%.1f < y < %.1f)", min_y, max_y));
    legend->SetTextSize(0.04);
    legend->SetBorderSize(0);

    hist_Dst->SetLineColor(kRed);
    hist_Dst->SetMarkerColor(kRed);
    hist_Dst->SetMarkerStyle(20); 
    hist_Dst->SetMarkerSize(0.4); 
    hist_Dst->GetYaxis()->SetRangeUser(0, 0.0087);

    hist_Dp->SetLineColor(kBlue);
    hist_Dp->SetMarkerColor(kBlue);
    hist_Dp->SetMarkerStyle(20);
    hist_Dp->SetMarkerSize(0.4); 

    hist_D0->SetLineColor(kBlack);
    hist_D0->SetMarkerColor(kBlack);
    hist_D0->SetMarkerStyle(20); 
    hist_D0->SetMarkerSize(0.4); 

    hist_Dst->SetStats(kFALSE);
    hist_Dp->SetStats(kFALSE);
    hist_D0->SetStats(kFALSE);

    hist_Dst->Draw("L");
    hist_Dp->Draw("L SAME");
    hist_D0->Draw("L SAME");

    int smoothing = 1000;
    hist_Dst->Smooth(smoothing);
    hist_Dp->Smooth(smoothing);
    hist_D0->Smooth(smoothing);
    hist_Dst->Sumw2(0);
    hist_Dp->Sumw2(0);
    hist_D0->Sumw2(0);
    hist_Dst->SetLineWidth(2); // Adjust the line width as needed
    hist_Dp->SetLineWidth(2);
    hist_D0->SetLineWidth(2);

    hist_Dst->SetFillStyle(0);
    hist_Dst->Draw("HIST");
    hist_Dp->SetFillStyle(0);
    hist_Dp->Draw("HIST SAME");
    hist_D0->SetFillStyle(0);
    hist_D0->Draw("HIST SAME");


    hist_Dst->SetTitle(Form("p_{T} Spectrum (%.1f < y < %.1f)", min_y, max_y));
    hist_Dst->GetYaxis()->SetTitle("Fraction of mesons");
    // hist_Dst->GetYaxis()->SetTitle("Number of mesons");
    hist_Dst->GetXaxis()->SetTitle("p_{T} (GeV/c)");
    legend->AddEntry(hist_Dst, "D^{*+}", "elp");
    legend->AddEntry(hist_Dp, "D^{+}", "elp");
    legend->AddEntry(hist_D0, "D^{0}", "elp");

    legend->Draw();
    c1->Draw();
    c1->Write();
    c1->Update();
    c1->SaveAs(Form("smooth_pT_all_%sy%s.pdf", min_y == 2.0 ? "2" : (min_y == 2.5 ? "2,5" : "0"), max_y == 2.5 ? "2,5" : (max_y == 3.0 ? "3" : "1")));
    file_Dst->Close();
    file_Dp->Close();
    file_D0->Close();
    file_output->Close();
}

