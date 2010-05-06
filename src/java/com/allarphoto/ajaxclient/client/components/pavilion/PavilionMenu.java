package com.lazerinc.ajaxclient.client.components.pavilion;

import com.google.gwt.user.client.Command;
import com.google.gwt.user.client.rpc.AsyncCallback;
import com.google.gwt.user.client.ui.HasHorizontalAlignment;
import com.google.gwt.user.client.ui.HorizontalPanel;
import com.google.gwt.user.client.ui.HTML;
import com.google.gwt.user.client.ui.Image;
import com.google.gwt.user.client.ui.Label;
import com.google.gwt.user.client.ui.MenuBar;
import com.google.gwt.user.client.ui.MenuItem;
import com.lazerinc.ajaxclient.client.AjaxSystem;
import com.lazerinc.ajaxclient.client.CommandFactory;
import com.lazerinc.ajaxclient.client.Services;
import com.lazerinc.ajaxclient.client.admin.LazerMenu;
import com.lazerinc.ajaxclient.client.beans.AjaxProductFamily;
import com.lazerinc.ajaxclient.client.components.BusyPopup;
import com.lazerinc.ajaxclient.client.components.SearchCenter;

public class PavilionMenu extends LazerMenu {
	protected boolean addBrowseMenu = true;

	public PavilionMenu(boolean vertical, CommandFactory c) {
		super(vertical, c);
		// TODO Auto-generated constructor stub
	}

	public PavilionMenu(CommandFactory c) {
		super(c);
		// TODO Auto-generated constructor stub
	}

	protected void addLeftHeader(CommandFactory cmd) {
		Image logo = new Image("pavilion/images/pavilion_gift_logo.jpg");
		logo.addStyleName("libLogo");
		add(logo);
	}
	
	/* --- clipboard for addRightHeader
	HTML pwdBy = new HTML("Powered by LazerWeb&trade;");
	pwdBy.addStyleName("poweredBy");
	add(pwdBy);
	*/
	
	// From Super
	/*
	HTML legend = new HTML("Icon<br/>Reference");
	legend.addStyleName("legend");
	add(legend);
	setCellHorizontalAlignment(legend, HasHorizontalAlignment.ALIGN_RIGHT);
	legend.addMouseListener(getLegendListener());
	*/
	
	protected void addRightHeader(CommandFactory cmd) {
		super.addRightHeader(cmd);
	}

	protected void addReturn(CommandFactory cmd) {
		setCellHorizontalAlignment(mainBar, HasHorizontalAlignment.ALIGN_LEFT);
	}

	protected void addBrowseMenu(CommandFactory cmd) {
		if(addBrowseMenu) super.addBrowseMenu(cmd);
	}
	
	/*
	protected void addHelpMenu(final CommandFactory cmd) {
		MenuItem item = new MenuItem("Help", cmd.get("Help"));
		item.setTitle("Jump to the Help section");
		mainBar.addItem(item);
	}
	*/

	protected void addMyStuffMenu(final CommandFactory cmd) {
		if (isLoggedIn) {
			MenuItem item;
			item = new MenuItem("My Account", getMyStuff(cmd));
			item
					.setTitle("View details and information specific to your account");
			mainBar.addItem(item);
		}
	}
	
	protected void addSupportFormLink(CommandFactory cmd) {
		MenuItem item = new MenuItem("User Tips", cmd.get("usertips"));
		item.setTitle("User Tips for Pavilion Gift Library.");
		mainBar.addItem(item);
	}

	protected void loadLibraryMenu(final CommandFactory cmd) {
		Services.getServices().libraryInfoService.getLibraries("read",
				new AsyncCallback() {

					public void onFailure(Throwable caught) {
						asyncCalls[LIBRARIES] = true;
					}

					public void onSuccess(Object result) {
						int items = 0;
						if (((AjaxProductFamily[]) result).length > 0) {
							final AjaxProductFamily[] families = (AjaxProductFamily[]) result;
							for (int i = 0; i < families.length; i++) {
								final int index = i;
								libraries.addItem(families[i]
										.getDescriptiveName(), new Command() {

									public void execute() {
										SearchCenter sc = new SearchCenter(
												AjaxSystem.getLabel(""));
										Services.getServices().mainPanel
												.setScreen(
														"Browse "
																+ families[index]
																		.getDescriptiveName(),
														sc,
														Services.getServices().factory
																.createComponent(
																		"SearchTree",
																		new Object[] {
																				sc,
																				families[index]
																						.getDescriptiveName() }),
														/* */ null, /* */
														Services.getServices().factory
																.createComponent(
																		"ShoppingCart",
																		new Object[] {
																				Services
																						.getServices().cart,
																				sc })/* ,
														null */ );
									}

								});
								items++;
							}
						}
						if (items > 1) {
							addBrowseMenu = true;
						} else {

							Services.getServices().libraryInfoService
									.getLibraries("admin", new AsyncCallback() {

										public void onFailure(Throwable caught) {

										}

										public void onSuccess(Object result) {
											if (((AjaxProductFamily[]) result).length > 0) {
												addBrowseMenu = true;
											}
										}

									});
						}
						asyncCalls[LIBRARIES] = true;
					}

				});
	}

}