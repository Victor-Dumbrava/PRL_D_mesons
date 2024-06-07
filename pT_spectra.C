#include <iomanip>

void pT_spectra() {

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

    double min_y = 2.5;
    double max_y = 3.0;

    TCut cuts1 = Form("Dstar_TRUEY >= %.1f && Dstar_TRUEY < %.1f && abs(Dstar_MC_MOTHER_ID) == 4", min_y, max_y); 
    TCut cuts2 = Form("Dp_TRUEY >= %.1f && Dp_TRUEY < %.1f && abs(Dp_MC_MOTHER_ID) == 4", min_y, max_y);
    TCut cuts3 = Form("Dp_TRUEY >= %.1f && Dp_TRUEY < %.1f && (abs(Dp_MC_MOTHER_ID) == 413)", min_y, max_y);
    TCut cuts4 = Form("D0_TRUEY >= %.1f && D0_TRUEY < %.1f && (abs(D0_MC_MOTHER_ID) == 4)", min_y, max_y);
    TCut cuts5 = Form("D0_TRUEY >= %.1f && D0_TRUEY < %.1f && (abs(D0_MC_MOTHER_ID) == 413 || abs(D0_MC_MOTHER_ID) == 423)", min_y, max_y);

    int nn = 300;
    int pmax = 14;
    TH1D* hist_Dst_ccbar = new TH1D("pT_Dst_ccbar", "Dstar_TRUEPT", nn, 0, pmax);
    TH1D* hist_Dp_ccbar = new TH1D("pT_Dp_ccbar", "Dp_TRUEPT", nn, 0, pmax);
    TH1D* hist_Dp_other = new TH1D("pT_Dp_other", "Dp_TRUEPT", nn, 0, pmax);
    TH1D* hist_D0_ccbar = new TH1D("pT_D0_ccbar", "D0_TRUEPT", nn, 0, pmax);
    TH1D* hist_D0_other = new TH1D("pT_D0_other", "D0_TRUEPT", nn, 0, pmax);

    Dst_tree->Draw("Dstar_TRUEPT / 1000 >> pT_Dst_ccbar", cuts1);
    Dp_tree->Draw("Dp_TRUEPT / 1000 >> pT_Dp_ccbar", cuts2);
    Dp_tree->Draw("Dp_TRUEPT / 1000 >> pT_Dp_other", cuts3);
    D0_tree->Draw("D0_TRUEPT / 1000 >> pT_D0_ccbar", cuts4);
    D0_tree->Draw("D0_TRUEPT / 1000 >> pT_D0_other", cuts5);

    hist_Dst_ccbar->Scale(1/hist_Dst_ccbar->GetEntries());
    hist_Dp_ccbar->Scale(1/hist_Dp_ccbar->GetEntries());
    hist_Dp_other->Scale(1/hist_Dp_other->GetEntries());
    hist_D0_ccbar->Scale(1/hist_D0_ccbar->GetEntries());
    hist_D0_other->Scale(1/hist_D0_other->GetEntries());

    hist_Dst_ccbar->Write();
    hist_Dp_ccbar->Write();
    hist_Dp_other->Write();
    hist_D0_ccbar->Write();
    hist_D0_other->Write();

    auto legend = new TLegend(0.55, 0.6, 0.85, 0.85);
    TCanvas *c1 = new TCanvas("c1", Form("pT Spectrum (%.1f < y < %.1f)", min_y, max_y));
    legend->SetTextSize(0.03);

    hist_Dst_ccbar->SetLineColor(kRed);
    hist_Dst_ccbar->SetMarkerColor(kRed);
    hist_Dst_ccbar->SetMarkerStyle(20); 
    hist_Dst_ccbar->SetMarkerSize(0.4); 
    // hist_Dst_ccbar->GetYaxis()->SetRangeUser(0, 0.015);

    hist_Dp_ccbar->SetLineColor(kBlue);
    hist_Dp_ccbar->SetMarkerColor(kBlue);
    hist_Dp_ccbar->SetMarkerStyle(20);
    hist_Dp_ccbar->SetMarkerSize(0.4); 

    hist_Dp_other->SetLineColor(kGreen);
    hist_Dp_other->SetMarkerColor(kGreen);
    hist_Dp_other->SetMarkerStyle(20); 
    hist_Dp_other->SetMarkerSize(0.4); 

    hist_D0_ccbar->SetLineColor(kBlack);
    hist_D0_ccbar->SetMarkerColor(kBlack);
    hist_D0_ccbar->SetMarkerStyle(20); 
    hist_D0_ccbar->SetMarkerSize(0.4); 

    hist_D0_other->SetLineColor(kMagenta);
    hist_D0_other->SetMarkerColor(kMagenta);
    hist_D0_other->SetMarkerStyle(20); 
    hist_D0_other->SetMarkerSize(0.4); 

    hist_Dst_ccbar->SetStats(kFALSE);
    hist_Dp_ccbar->SetStats(kFALSE);
    hist_Dp_other->SetStats(kFALSE);
    hist_D0_ccbar->SetStats(kFALSE);
    hist_D0_other->SetStats(kFALSE);

    hist_Dst_ccbar->Draw("L");
    hist_Dp_ccbar->Draw("L SAME");
    hist_Dp_other->Draw("L SAME");
    hist_D0_ccbar->Draw("L SAME");
    hist_D0_other->Draw("L SAME");

// int smoothing = 1000;
// hist_Dst_ccbar->Smooth(smoothing);
// hist_Dp_ccbar->Smooth(smoothing);
// hist_Dp_other->Smooth(smoothing);
// hist_D0_ccbar->Smooth(smoothing);
// hist_D0_other->Smooth(smoothing);
// hist_Dst_ccbar->Sumw2(0);
// hist_Dp_ccbar->Sumw2(0);
// hist_Dp_other->Sumw2(0);
// hist_D0_ccbar->Sumw2(0);
// hist_D0_other->Sumw2(0);
// hist_Dst_ccbar->SetLineWidth(2); // Adjust the line width as needed
// hist_Dp_ccbar->SetLineWidth(2);
// hist_Dp_other->SetLineWidth(2);
// hist_D0_ccbar->SetLineWidth(2);
// hist_D0_other->SetLineWidth(2);

    // hist_Dst_ccbar->SetFillStyle(0);
    // hist_Dst_ccbar->Draw("HIST");
    // hist_Dp_ccbar->SetFillStyle(0);
    // hist_Dp_ccbar->Draw("HIST SAME");
    // hist_Dp_other->SetFillStyle(0);
    // hist_Dp_other->Draw("HIST SAME");
    // hist_D0_ccbar->SetFillStyle(0);
    // hist_D0_ccbar->Draw("HIST SAME");
    // hist_D0_other->SetFillStyle(0);
    // hist_D0_other->Draw("HIST SAME");

    hist_Dst_ccbar->SetTitle(Form("p_{T} Spectrum (%.1f < y < %.1f)", min_y, max_y));
    hist_Dst_ccbar->GetYaxis()->SetTitle("Fraction of mesons");
    // hist_Dst_ccbar->GetYaxis()->SetTitle("Number of mesons");
    hist_Dst_ccbar->GetXaxis()->SetTitle("p_{T} (GeV/c)");
    legend->AddEntry(hist_Dst_ccbar, "D^{*+} from c or #bar{c}", "elp");
    legend->AddEntry(hist_Dp_ccbar, "D^{+} from c or #bar{c}", "elp");
    legend->AddEntry(hist_Dp_other, "D^{+} from D^{*+}", "elp");
    legend->AddEntry(hist_D0_ccbar, "D^{0} from c or #bar{c}", "elp");
    legend->AddEntry(hist_D0_other, "D^{0} from D^{*+} or D^{*0}", "elp");

    legend->Draw();
    c1->Draw();
    c1->Write();
    c1->Update();
    c1->SaveAs(Form("new_pT_spectra_%sy%s.pdf", min_y == 2.0 ? "2" : (min_y == 2.5 ? "2,5" : "0"), max_y == 2.5 ? "2,5" : (max_y == 3.0 ? "3" : "1")));
    file_Dst->Close();
    file_Dp->Close();
    file_D0->Close();
    file_output->Close();
}

