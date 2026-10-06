{
    // set seed to get consistent random values
    gRandom->SetSeed(4357);

    const int color_list[] = {kBlue+1, kOrange+8, kGreen+2, kRed-4, kYellow+1,
        kPink+7, kAzure+5};
    const int color_list_size = sizeof(color_list) / sizeof(color_list[0]);

    const int marker_list[] = {20, 21, 22, 23, 29, 43, 47, 33};
    const int marker_list_size = sizeof(marker_list) / sizeof(marker_list[0]);


    // create a single canvas and divide in a 2 by 2 grid
    TCanvas *c = new TCanvas("c1", "Multiplot template", 1200, 800);
    c->Divide(2, 2);

    /*
     * 1,1
     *
     * This is an example of how you can draw multiple kinds of plots in the
     * same frame with a legend. We'll draw a TGraph, two TH1F and a TF1
     */
    // first a graph
    const int N1 = 10;
    float x1[N1], y1[N1];
    for (int i = 0; i < N1; i++) {
        x1[i] = gRandom->Uniform(0, 10);
        y1[i] = gRandom->Uniform(0, 10);
    }
    // the graph's parameters are
    //     TGraph(<# of points>, <array of x's>, <array of y's>)
    TGraph *g1 = new TGraph(N1, x1, y1);
    g1->SetMarkerStyle(marker_list[0]);
    g1->SetMarkerColor(color_list[0]);

    // Then histograms. There are two ways of creating histograms.
    // The conventional way consists of defining bins, and then populating
    // the histogram with values (root will make sure each value is added to
    // the correct bin).
    // Otherwise, you can treat the histogram like a bar plot and set each
    // bin's height manueally.
    // The histogram's parameters are
    //     TH1F(<name>, <title>, <# of bins>, <low edge of first bin>, <upper edge of last bin (not included in last bin)>)
    TH1F *hist_traditional = new TH1F("hist_traditional", "Hist (trad);bins;count", 5, 0, 4);
    // Let's fill this histogram with 40 values
    for (int i = 0; i < 40; i++) {
        float value = gRandom->Uniform(0, 4);
        hist_traditional->Fill(value);
    }
    hist_traditional->SetLineColor(color_list[1]);
    hist_traditional->SetFillColorAlpha(color_list[1], 0.10);

    // create the "bar plot" style histogram
    TH1F *hist_bar = new TH1F("hist_bar", "Hist (conv);bins;count", 5, 6, 10);
    // Let's set the histogram's bin heights
    for (int i = 0; i < hist_bar->GetNbinsX(); i++) {
        float value = gRandom->Uniform(0, 10);
        // notice that I used i+1. The valid data bins are 1 to N. 0 is the
        // underflow bin and N+1 is the overflow bin
        hist_bar->SetBinContent(i+1, value);
    }
    hist_bar->SetLineColor(color_list[2]);
    hist_bar->SetFillColorAlpha(color_list[2], 0.10);

    // finally, the tf1
    TF1 *tf1 = new TF1("tf1", "[0]*pow(x-[4],3) + [1]*pow(x-[4],2) + [2]*(x-[4]) + [3]", 0, 10);
    tf1->SetParameters(0.5, 2.5, 0.3, 1, 6);
    tf1->SetLineColor(color_list[3]);

    // then, let's add the legend
    // it has parameters
    //     TLegend(x1, y1, x2, y2)
    // and the coordinates are in fractions of the pad (from 0 to 1)
    TLegend *leg1 = new TLegend(0.73, 0.73, 0.93, 0.93);
    leg1->SetHeader("Legend title", "C"); // C centers the text
    leg1->AddEntry(g1, "TGraph", "p");
    leg1->AddEntry(hist_traditional, "TH1F (trad)", "f");
    leg1->AddEntry(hist_bar, "TH1F (bar)", "f");
    leg1->AddEntry(tf1, "TF1", "l");

    c->cd(1);
    // Once we've cd'ed into the right pad of the canvas, we can use gPad to
    // draw the frame, without needing to create a new graph ad-hoc. The handle
    // no manage that empty frame is returned in the form of a TH1F, which we
    // could later use to modify the frame further, but even if the variable
    // frame is of type TH1F, it shouldn't be treated like an actual histogram.
    // The arguments for DrawFrame are:
    //     gPad->DrawFrame(x_min, y_min, x_max, y_max, "<title>;<x label>;<y label>")
    TH1F *frame = gPad->DrawFrame(0, 0, 10, 10, "Title 1;x label 1;y label 2");
    hist_traditional->Draw("SAME");
    hist_bar->Draw("SAME");
    tf1->Draw("SAME");
    g1->Draw("P SAME");
    leg1->Draw();

    /*
     * 1,2
     *
     * A multigraph lets us create multiple TGraphs that should be drawn in
     * the same plot, grouping them conveniently. For example, axis ranges
     * are calculated automatically!
     */
    TMultiGraph *mg = new TMultiGraph();
    for (int i = 0; i < 5; i++) {
        // define some paramteres
        const int N = 10;
        const float dx = 1 + i*0.2;
        const float a = 1 + i*0.2;

        // create the graph arrays
        float x[N], y[N];
        for (int j = 0; j < N; j++) {
            x[j] = dx * j;
            y[j] = -a * x[j] * (x[j] - dx*(N-1));
        }

        // create the TGraph, change its style and add it to the TMultiGraph
        TGraph *g = new TGraph(N, x, y);
        g->SetMarkerStyle(marker_list[i % marker_list_size]);
        g->SetMarkerColor(color_list[i % color_list_size]);
        g->SetLineColor(color_list[i % color_list_size]);
        mg->Add(g);
    }

    // plot the TMultiGraph
    c->cd(2);
    // extra: set grid for the current pad
    gPad->SetGrid();
    mg->SetTitle("Title 2;x label 2;y label 2");
    // To draw, A means create the frame, P means draw the markers
    // L means draw a line between the markers. You can delete any
    // of these characters to remove that functionality
    mg->Draw("APL");

    /*
     * 2D histograms (TH2F): x and y are binned, and each bin holds a count.
     * The parameters are
     *     TH2F(<name>, <title>, <# x bins>, <x low>, <x high>,
     *                           <# y bins>, <y low>, <y high>)
     */
    const int n_entries = 5000;
    const int n_bins = 20;
    const float lo = -4, hi = 4;
    TH2F *hist2d = new TH2F("hist2d", "Title 3;x label 3;y label 3;count",
                            n_bins, lo, hi, n_bins, lo, hi);
    hist2d->SetStats(0); // hide the statistics box
    // fill it with points from a 2D gaussian
    for (int i = 0; i < n_entries; i++) {
        hist2d->Fill(gRandom->Gaus(0, 1), gRandom->Gaus(0, 1));
    }

    c->cd(3);
    gPad->SetRightMargin(0.15); // make room for the color scale
    // COLZ draws a heatmap with a color scale (Z) on the right
    hist2d->Draw("COLZ");

    /*
     * 2,2
     * For a 3D scatter plot, you can use a TGraph2D. It works like a TGraph,
     * but with a z array too.
     *     TGraph2D(<# of points>, <array of x's>, <array of y's>, <array of z's>)
     */
    const float bin_area = ((hi - lo) / n_bins) * ((hi - lo) / n_bins);
    const int N3 = 150;
    double x3[N3], y3[N3], z3[N3];
    for (int i = 0; i < N3; i++) {
        x3[i] = gRandom->Uniform(-3.5, 3.5);
        y3[i] = gRandom->Uniform(-3.5, 3.5);
        z3[i] = gRandom->Uniform(10, hist2d->GetBinContent(n_bins/2+1, n_bins/2+1 * 1.2));
    }
    TGraph2D *g2d = new TGraph2D(N3, x3, y3, z3);
    g2d->SetMarkerStyle(marker_list[0]);
    g2d->SetMarkerSize(0.6);
    g2d->SetMarkerColor(color_list[3]);

    // We draw a copy of the histogram so that changing its style or z range
    // here doesn't affect the heatmap in pad 3
    TH2F *hist_lego = (TH2F*)hist2d->Clone("hist_lego");
    hist_lego->SetTitle("Title 4;x label 4;y label 4;count");
    hist_lego->SetFillColor(kGray);
    // make sure the z axis is tall enough for both the bars and the points
    hist_lego->SetMaximum(1.15 * TMath::Max(hist2d->GetMaximum(), g2d->GetZmax()));

    c->cd(4);
    // LEGO1 draws the histogram as 3D bars (the histogram creates the 3D frame)
    hist_lego->Draw("LEGO1");
    // P draws the markers, SAME draws on top of the existing 3D frame
    g2d->Draw("P SAME");

    // when running in batch mode (e.g. in CI), save the canvas to a file
    if (gROOT->IsBatch()) {
        c->SaveAs("multiplot.png");
    }
}
