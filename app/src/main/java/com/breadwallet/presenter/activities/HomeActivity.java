package com.breadwallet.presenter.activities;

import android.app.Activity;
import android.content.Intent;
import android.os.Bundle;
import android.support.annotation.Nullable;
import android.support.v7.widget.LinearLayoutManager;
import android.support.v7.widget.RecyclerView;
import android.view.View;

import com.breadwallet.R;
import com.breadwallet.presenter.customviews.BRText;
import com.breadwallet.tools.adapter.WalletListAdapter;
import com.breadwallet.tools.listeners.RecyclerItemClickListener;
import com.breadwallet.tools.manager.BRSharedPrefs;
import com.breadwallet.tools.sqlite.CurrencyDataSource;
import com.breadwallet.tools.threads.BRExecutor;
import com.breadwallet.tools.util.CurrencyUtils;
import com.breadwallet.wallet.WalletsMaster;
import com.breadwallet.wallet.abstracts.BaseWalletManager;

import java.math.BigDecimal;
import java.util.ArrayList;

/**
 * Created by byfieldj on 1/17/18.
 * <p>
 * Dummy Home activity to simulate navigating to and from the new Currency screen
 */

public class HomeActivity extends Activity {

    private RecyclerView mWalletRecycler;
    private WalletListAdapter mAdapter;
    private BRText fiatTotal;

    private static HomeActivity app;

    public static HomeActivity getApp() {
        return app;
    }


    @Override
    protected void onCreate(@Nullable Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_home);

        WalletsMaster.getInstance(this).initWallets(this);

        ArrayList<BaseWalletManager> walletList = new ArrayList<>();

        walletList.addAll(WalletsMaster.getInstance(this).getAllWallets());

        mWalletRecycler = findViewById(R.id.rv_wallet_list);
        fiatTotal = findViewById(R.id.total_assets_usd);
        mAdapter = new WalletListAdapter(this, walletList);

        mWalletRecycler.setLayoutManager(new LinearLayoutManager(this));
        mWalletRecycler.setAdapter(mAdapter);

        mWalletRecycler.addOnItemTouchListener(new RecyclerItemClickListener(this, mWalletRecycler, new RecyclerItemClickListener.OnItemClickListener() {
            @Override
            public void onItemClick(View view, int position, float x, float y) {
                BRSharedPrefs.putCurrentWalletIso(HomeActivity.this, mAdapter.getItemAt(position).getIso(HomeActivity.this));
                Intent newIntent = new Intent(HomeActivity.this, WalletActivity.class);
                startActivity(newIntent);
            }

            @Override
            public void onLongItemClick(View view, int position) {

            }
        }));

    }

    @Override
    protected void onResume() {
        super.onResume();
        app = this;
        BRSharedPrefs.putCurrentWalletIso(HomeActivity.this, "");
        updateUi();
        CurrencyDataSource.getInstance(this).addOnDataChangedListener(new CurrencyDataSource.OnDataChanged() {
            @Override
            public void onChanged() {
                BRExecutor.getInstance().forMainThreadTasks().execute(new Runnable() {
                    @Override
                    public void run() {
                        updateUi();
                    }
                });

            }
        });
    }

    private void updateUi() {
        BigDecimal fiatTotalAmount = WalletsMaster.getInstance(this).getAgregatedFiatBalance(this);
        fiatTotal.setText(CurrencyUtils.getFormattedCurrencyString(this, BRSharedPrefs.getPreferredFiatIso(this), fiatTotalAmount));
        mAdapter.notifyDataSetChanged();
    }

}
