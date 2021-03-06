package com.allarphoto.ajaxclient.client.components.mcc;

import java.util.Iterator;
import java.util.List;

import com.google.gwt.user.client.rpc.AsyncCallback;
import com.google.gwt.user.client.ui.DockPanel;
import com.google.gwt.user.client.ui.Label;
import com.google.gwt.user.client.ui.TabPanel;
import com.google.gwt.user.client.ui.Widget;
import com.allarphoto.ajaxclient.client.AjaxSystem;
import com.allarphoto.ajaxclient.client.Services;
import com.allarphoto.ajaxclient.client.beans.AjaxCart;
import com.allarphoto.ajaxclient.client.beans.OrderVerificationPackage;
import com.allarphoto.ajaxclient.client.components.CheckoutPanel;
import com.allarphoto.ajaxclient.client.components.PopupWarning;
import com.allarphoto.ajaxclient.client.components.VerifyScreen;

public class MccVerifyScreen extends VerifyScreen {

	public MccVerifyScreen(AjaxCart c, CheckoutPanel checkout) {
		super(c, checkout);
	}

	protected Widget getOrderDisplay(List requests) {
		return new Label("");
	}

	protected void createDisplay() {
		Iterator iter = packages.iterator();
		while (iter.hasNext()) {
			OrderVerificationPackage ovp = (OrderVerificationPackage) iter
					.next();
			DockPanel dp = new DockPanel();
			addLicenseInfo(dp, ovp.getLicenseText());
			addVerifyFormElements(dp, ovp.getFields());
			if (ovp.getLicenseText() != null
					&& ovp.getLicenseText().length() > 0)
				addControlButtons(dp);
			add(dp);
			setSpacing(10);
			tabbed.add(new Label(), ovp.getFamily());
		}
		add(AjaxSystem.getText("(red text = required field)"));
		tabbed.selectTab(0);
	}

	protected void init() {
		tabbed = new TabPanel();
		Services.getServices().orderService.getOrderVerificationPackages(cart,
				new AsyncCallback() {

					public void onFailure(Throwable arg0) {
						new PopupWarning(
								"Failed to get order verification package info "
										+ arg0);

					}

					public void onSuccess(Object results) {
						packages = (List) results;
						createDisplay();
					}

				});
		add(AjaxSystem.getTitle("Checkout"));
		setSpacing(20);

	}

}
