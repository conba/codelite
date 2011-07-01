#include "editorsettingsdockingwidows.h"
#include "editor_config.h"

EditorSettingsDockingWindows::EditorSettingsDockingWindows( wxWindow* parent )
		: EditorSettingsDockingWindowsBase( parent )
{
	OptionsConfigPtr options = EditorConfigST::Get()->GetOptions();
	
	m_checkBoxHideOutputPaneOnClick->SetValue        (options->GetHideOutpuPaneOnUserClick());
	m_checkBoxHideOutputPaneNotIfBuild->SetValue     (options->GetHideOutputPaneNotIfBuild());
	m_checkBoxHideOutputPaneNotIfErrors->SetValue    (options->GetHideOutputPaneNotIfErrors());
	m_checkBoxHideOutputPaneNotIfSearch->SetValue    (options->GetHideOutputPaneNotIfSearch());
	m_checkBoxHideOutputPaneNotIfReplace->SetValue   (options->GetHideOutputPaneNotIfReplace());
	m_checkBoxHideOutputPaneNotIfReferences->SetValue(options->GetHideOutputPaneNotIfReferences());
	m_checkBoxHideOutputPaneNotIfOutput->SetValue    (options->GetHideOutputPaneNotIfOutput());
	m_checkBoxHideOutputPaneNotIfDebug->SetValue     (options->GetHideOutputPaneNotIfDebug());
	m_checkBoxHideOutputPaneNotIfTrace->SetValue     (options->GetHideOutputPaneNotIfTrace());
	m_checkBoxHideOutputPaneNotIfTasks->SetValue     (options->GetHideOutputPaneNotIfTasks());
	m_checkBoxFindBarAtBottom->SetValue              (options->GetFindBarAtBottom());
	m_checkBoxOutputPaneCanDock->SetValue            (options->GetOutputPaneDockable());
	m_checkBoxShowDebugOnRun->SetValue               (options->GetShowDebugOnRun());
	m_radioBoxHint->SetSelection                     (options->GetDockingStyle());
	
	int tabStyle (0); // Classic
	m_startingFlags = OptionsConfig::TabClassic;
	if(options->GetOptions() & OptionsConfig::TabCurved) {
		tabStyle = 2;
		m_startingFlags = OptionsConfig::TabCurved;
		
	} else if(options->GetOptions() & OptionsConfig::TabGlossy) {
		tabStyle = 1;
		m_startingFlags = OptionsConfig::TabGlossy;
	}
	
	m_endFlags = m_startingFlags;
	m_radioBoxTabControlStyle->SetSelection(tabStyle);
#if CL_USE_NATIVEBOOK
	m_radioBoxTabControlStyle->Enable(false);
#endif
	m_checkBoxHideOutputPaneNotIfDebug->Connect( wxEVT_UPDATE_UI, wxUpdateUIEventHandler( EditorSettingsDockingWindows::OnHideOutputPaneNotIfDebugUI ), NULL, this );
}

void EditorSettingsDockingWindows::Save(OptionsConfigPtr options)
{
	options->SetHideOutpuPaneOnUserClick( m_checkBoxHideOutputPaneOnClick->IsChecked() );
	options->SetHideOutputPaneNotIfBuild( m_checkBoxHideOutputPaneNotIfBuild->IsChecked() );
	options->SetHideOutputPaneNotIfErrors( m_checkBoxHideOutputPaneNotIfErrors->IsChecked() );
	options->SetHideOutputPaneNotIfSearch( m_checkBoxHideOutputPaneNotIfSearch->IsChecked() );
	options->SetHideOutputPaneNotIfReplace( m_checkBoxHideOutputPaneNotIfReplace->IsChecked() );
	options->SetHideOutputPaneNotIfReferences( m_checkBoxHideOutputPaneNotIfReferences->IsChecked() );
	options->SetHideOutputPaneNotIfOutput( m_checkBoxHideOutputPaneNotIfOutput->IsChecked() );
	options->SetHideOutputPaneNotIfDebug( m_checkBoxHideOutputPaneNotIfDebug->IsChecked() );
	options->SetHideOutputPaneNotIfTrace( m_checkBoxHideOutputPaneNotIfTrace->IsChecked() );
	options->SetHideOutputPaneNotIfTasks( m_checkBoxHideOutputPaneNotIfTasks->IsChecked() );
	options->SetFindBarAtBottom( m_checkBoxFindBarAtBottom->IsChecked() );
	options->SetOutputPaneDockable( m_checkBoxOutputPaneCanDock->IsChecked() );
	options->SetShowDebugOnRun( m_checkBoxShowDebugOnRun->IsChecked() );
	options->SetDockingStyle( m_radioBoxHint->GetSelection() );
	
	size_t flags(options->GetOptions());
	m_endFlags = 0;
	// set the tab control options:
	////////////////////////////////////
	
	// Clear the current tab control style
	flags &= ~OptionsConfig::TabAll;
	
	switch(m_radioBoxTabControlStyle->GetSelection()) {
	case 1: // glossy
		flags |= OptionsConfig::TabGlossy;
		m_endFlags |= OptionsConfig::TabGlossy;
		break;
	case 2: // curved
		flags |= OptionsConfig::TabCurved;
		m_endFlags |= OptionsConfig::TabCurved;
		break;
	default:
	case 0: // classic
		flags |= OptionsConfig::TabClassic;
		m_endFlags |= OptionsConfig::TabClassic;
		break;
	}
	options->SetOptions(flags);
}

void EditorSettingsDockingWindows::OnHideOutputPaneNotIfDebugUI(wxUpdateUIEvent& event)
{
	event.Enable( m_checkBoxHideOutputPaneOnClick->IsChecked() );
}

bool EditorSettingsDockingWindows::IsRestartRequired()
{
	return m_startingFlags != m_endFlags;
}
