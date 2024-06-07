void openInputFiles(std::vector<TGraphAsymmErrors*>& graphs, int mixed = 0, bool Dst = false, bool indirect = false, bool alice = false) {
    // boolean params are: false/true (for D0/Dst), false/true (for ccbar/other sources), false/true (for lchb/alice) respectively

    std::string fileName;
    if (mixed == 0) {
        if (alice) {
            if (indirect) {
                if (Dst) {
                    fileName = "alice_other_plotratio_Dstar_Dp.root";
                } else {
                    fileName = "alice_indirect_from_Dst_D0_Dp.root";
                }
            } else {
                if (Dst) {
                    fileName = "alice_direct_hadronization_plotratio_Dstar_Dp.root";
                } else {
                    fileName = "alice_direct_hadronization_plotratio_D0_Dp.root";
                }
            }
        } else {
            if (indirect) {
                if (Dst) {
                    fileName = "plotratio_other_Dstar_Dp.root";
                } else {
                    fileName = "indirect_from_Dst_D0_Dp.root";
                }
            } else {
                if (Dst) {
                    fileName = "plotratio_direct_hadronization_Dstar_Dp.root";
                } else {
                    fileName = "plotratio_direct_hadronization_D0_Dp.root";
                }
            }
        }
    }
    else {
        if (alice) {
            fileName = Form("alice_mixed%i_plotratio_Dstar_Dp.root", mixed);
        } else {
            fileName = Form("mixed%i_Dstar_Dp.root", mixed);
        }
    }

    auto file = new TFile(fileName.c_str(), "READ");

    if (!file || file->IsZombie()) {
        std::cerr << "Error: Could not open input file " << fileName << "." << std::endl;
        delete file; 
        file = nullptr;
        return;
    }

    int numGraphs = (alice ? 1 : 6);

    for (size_t i = 0; i < numGraphs; ++i) {
        std::string graphName = "graph" + (alice ? "" : "_" + std::to_string(i));
        TGraphAsymmErrors* graph = (TGraphAsymmErrors*)file->Get(graphName.c_str());
        int nPoints = graph->GetN();  
        double *x = graph->GetX();
        double *y = graph->GetY();
        double *exl = graph->GetEXlow();
        double *exh = graph->GetEXhigh();
        double *eyl = graph->GetEYlow();
        double *eyh = graph->GetEYhigh();
        double *newX = new double[nPoints];
        double *newEXlow = new double[nPoints];
        double *newEXhigh = new double[nPoints];
        for (int i = 0; i < nPoints; ++i) {
            newX[i] = x[i] * 0.001;       // Convert MeV/c to GeV/c
            newEXlow[i] = exl[i] * 0.001; // Convert MeV/c to GeV/c
            newEXhigh[i] = exh[i] * 0.001; // Convert MeV/c to GeV/c
        }
        TGraphAsymmErrors *new_graph = new TGraphAsymmErrors(nPoints, newX, y, newEXlow, newEXhigh, eyl, eyh);
        new_graph->GetXaxis()->SetTitle("p_{T} (GeV/c)");
        double xMin = graph->GetXaxis()->GetXmin() * 0.001;
        double xMax = graph->GetXaxis()->GetXmax() * 0.001;
        new_graph->GetXaxis()->SetLimits(xMin, xMax);
        new_graph->GetXaxis()->SetTitleSize(0.06);
        new_graph->GetYaxis()->SetTitleSize(0.06);
        new_graph->GetXaxis()->SetLabelSize(0.05);
        new_graph->GetYaxis()->SetLabelSize(0.05);
        new_graph->SetLineWidth(2);
        new_graph->GetYaxis()->SetRangeUser(0.2, 3.2);
        graphs.push_back(new_graph);
    }

    std::cout << "Input file " << fileName << " read successfully." << std::endl;
}

void slope_attempt_Dst_Dp() {

    std::vector<TGraphAsymmErrors*> graphs;
    std::vector<TString> labels = { "2.0 < y < 2.5 (D^{*+}: other, D^{+}: c / #bar{c})",
                                    "2.5 < y < 3.0 (D^{*+}: other, D^{+}: c / #bar{c})",
                                    "0.0 < y < 1.0 (D^{*+}: other, D^{+}: c / #bar{c})", // more here but this is useless
                                    "2.0 < y < 2.5 (D^{*+}: c / #bar{c}, D^{+}: D^{*0} or D^{*+})",
                                    "2.5 < y < 3.0 (D^{*+}: c / #bar{c}, D^{+}: D^{*0} or D^{*+})",
                                    "0.0 < y < 1.0 (D^{*+}: c / #bar{c}, D^{+}: D^{*0} or D^{*+})"};
    openInputFiles(graphs, 1, true, false, false);
    openInputFiles(graphs, 1, true, false, true);
    openInputFiles(graphs, 2, true, false, false);
    openInputFiles(graphs, 2, true, false, true);

    std::cout << "Starting processing." << std::endl;
    int colors[] = {1, 2, 48, 6, 38, 42, 40};
    int markers[] = {21, 71, 89, 107, 72, 90, 108};
    gStyle->SetTitleFontSize(0.04); 

    TFile *output_file = new TFile("slope_attempt_Dst_Dp.root", "RECREATE");
    if (!output_file || output_file->IsZombie()) {
        std::cerr << "Error: Unable to create output file 'slope_attempt_Dst_Dp.root'.\n";
        delete output_file;
        return;
    }

    output_file->cd();
    TCanvas *c1 = new TCanvas();
    TLegend *legend = new TLegend(0.2, 0.25, 0.6, 0.42);
    legend->SetTextSize(0.035);
    legend->SetBorderSize(0);

    if (!graphs.empty()) {
        graphs[0]->Draw("AP"); 
        graphs[0]->GetYaxis()->SetTitle("D^{*+} / D^{+}");
        graphs[0]->GetXaxis()->SetTitleSize(0.04);
        graphs[0]->GetYaxis()->SetTitleSize(0.04);
        graphs[0]->GetXaxis()->SetLabelSize(0.04);
        graphs[0]->GetYaxis()->SetLabelSize(0.04);
        graphs[0]->GetXaxis()->SetTitleOffset(1.2);
        graphs[0]->GetYaxis()->SetTitleOffset(1);
        graphs[0]->GetYaxis()->SetRangeUser(0.2, 3.2);
        graphs[0]->SetTitle("p_{T} trend of the ratio D^{*+} / D^{+} (different sources)");
        gPad->SetLeftMargin(0.10);
        gPad->SetRightMargin(0.10);
        gPad->SetBottomMargin(0.10);
        gPad->SetTopMargin(0.10);
        graphs[0]->SetMarkerStyle(20);
        graphs[0]->SetMarkerColor(colors[4]);
        graphs[0]->SetLineColor(colors[4]);
        legend->AddEntry(graphs[0], "D^{*+} from other sources, D^{+} from c or #bar{c}", "elp"); 
    }

    for (size_t i = 1; i < 7; ++i) {
        graphs[i]->Draw("P SAME"); 
        graphs[i]->SetMarkerStyle(markers[4]);
        graphs[i]->SetMarkerColor(colors[4]);
        graphs[i]->SetLineColor(colors[4]);
    }

    TList graphCollection;
    for (int i=7; i<14; i++) {
        graphCollection.Add(graphs[i]);
    }
    TGraphAsymmErrors *mergedGraph = new TGraphAsymmErrors();
    mergedGraph->Merge(static_cast<TCollection*>(&graphCollection));
    mergedGraph->Draw("P SAME"); 
    mergedGraph->SetMarkerStyle(markers[0]);
    mergedGraph->SetMarkerColor(32);
    mergedGraph->SetMarkerSize(1.2);
    mergedGraph->SetLineColor(32);
    mergedGraph->SetLineWidth(2);
    // Draw the line plot
    // mergedGraph->Draw("AL");

    // Draw the markers with black contour
    mergedGraph->Draw("P SAME");

    legend->AddEntry(mergedGraph, "D^{*+} from c or #bar{c}, D^{+} from feeddown of D^{*0} or D^{*+}", "elp");

    TF1 *linear_fit = new TF1("fitFunc", "pol1", 0, 8);
    linear_fit->SetLineColor(46);
    linear_fit->SetLineWidth(5);
    mergedGraph->Fit(linear_fit, "R");
    linear_fit->Draw("same");
    legend->AddEntry(linear_fit, "linear fit (0 < p_{T} < 8 GeV/c)");

    legend->Draw();
    c1->Update();
    c1->Write();
    c1->SaveAs("slope_attempt_Dst_Dp.pdf");
    output_file->Close();
    delete output_file;

    std::cout << "Done." << std::endl;

    /* 
        ****************************************
        Minimizer is Minuit2 / Migrad
        Chi2                      =      58.8773
        NDf                       =           54
        Edm                       =  4.92606e-10
        NCalls                    =           95
        p0                        =      1.49612   +/-   0.0158372   
        p1                        =     0.104526   +/-   0.00422176 
    */

}
