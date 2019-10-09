package com.breadwallet.ui.managewallets

import com.breadwallet.breadbox.BreadBox
import com.breadwallet.breadbox.applyDisplayOrder
import com.breadwallet.breadbox.isErc20
import com.breadwallet.ext.bindConsumerIn
import com.platform.interfaces.AccountMetaDataProvider
import com.spotify.mobius.Connection
import com.spotify.mobius.functions.Consumer
import kotlinx.coroutines.CoroutineScope
import kotlinx.coroutines.Dispatchers
import kotlinx.coroutines.SupervisorJob
import kotlinx.coroutines.cancelChildren
import kotlinx.coroutines.flow.launchIn
import kotlinx.coroutines.flow.map
import kotlinx.coroutines.flow.mapLatest
import com.breadwallet.crypto.Wallet as CryptoWallet

class ManageWalletsEffectHandler(
    private val output: Consumer<ManageWalletsEvent>,
    private val breadBox: BreadBox,
    private val acctMetaDataProvider: AccountMetaDataProvider
) : Connection<ManageWalletsEffect>, CoroutineScope {

    override val coroutineContext = SupervisorJob() + Dispatchers.Default

    override fun accept(value: ManageWalletsEffect) {
        when (value) {
            ManageWalletsEffect.LoadEnabledWallets -> loadEnabledWallets()
            is ManageWalletsEffect.UpdateWallet -> updateEnabledWallets(
                value.currencyId,
                value.isEnabled
            )
            is ManageWalletsEffect.ReorderWallets -> reorderWallets(value.wallets)
        }
    }

    override fun dispose() {
        coroutineContext.cancelChildren()
    }

    private fun loadEnabledWallets() {
        breadBox.wallets()
            .applyDisplayOrder(acctMetaDataProvider.enabledWallets())
            .mapLatest { wallets -> wallets.map { it.asWallet() } }
            .map { ManageWalletsEvent.OnWalletsUpdated(it) }
            .bindConsumerIn(output, this)
    }

    private fun updateEnabledWallets(currencyId: String, isEnabled: Boolean) {
        when {
            isEnabled -> acctMetaDataProvider.enableWallet(currencyId).launchIn(this)
            else -> acctMetaDataProvider.disableWallet(currencyId).launchIn(this)
        }
    }

    private fun reorderWallets(wallets: List<String>) {
        acctMetaDataProvider.reorderWallets(wallets).launchIn(this)
    }

    private fun CryptoWallet.asWallet(): Wallet {
        return Wallet(currency.name, currency.code, currency.isErc20(), currency.uids)
    }
}
