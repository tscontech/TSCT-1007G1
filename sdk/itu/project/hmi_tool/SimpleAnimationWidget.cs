﻿using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Drawing;
using System.Globalization;
using System.Linq;
using System.Text;
using System.Windows.Forms;

namespace GUIDesigner
{
    class SimpleAnimationWidget : Panel, IWidget
    {
        public SimpleAnimationWidget()
        {
            base.Margin = new Padding(0, 0, 0, 0);
            this.Delay = 1;
            this.TotalFrame = 10;
            this.MoveAnimation = true;
            this.RotateAnimation = true;
            this.ScaleAnimation = true;
            this.Cycling = false;
            this.ScaleOnCenter = false;
            this.AlwaysVisible = false;
            this.Action01 = new WidgetAction(ITU.WidgetActionType.None, WidgetEvent.Stopped, "", "");
            this.Action02 = new WidgetAction(ITU.WidgetActionType.None, WidgetEvent.Stopped, "", "");
            this.Action03 = new WidgetAction(ITU.WidgetActionType.None, WidgetEvent.Stopped, "", "");
            this.Action04 = new WidgetAction(ITU.WidgetActionType.None, WidgetEvent.Stopped, "", "");
            this.Action05 = new WidgetAction(ITU.WidgetActionType.None, WidgetEvent.Stopped, "", "");
            this.Action06 = new WidgetAction(ITU.WidgetActionType.None, WidgetEvent.Stopped, "", "");
            this.Action07 = new WidgetAction(ITU.WidgetActionType.None, WidgetEvent.Stopped, "", "");
            this.Action08 = new WidgetAction(ITU.WidgetActionType.None, WidgetEvent.Stopped, "", "");
            this.Action09 = new WidgetAction(ITU.WidgetActionType.None, WidgetEvent.Stopped, "", "");
            this.Action10 = new WidgetAction(ITU.WidgetActionType.None, WidgetEvent.Stopped, "", "");
            this.Action11 = new WidgetAction(ITU.WidgetActionType.None, WidgetEvent.Stopped, "", "");
            this.Action12 = new WidgetAction(ITU.WidgetActionType.None, WidgetEvent.Stopped, "", "");
            this.Action13 = new WidgetAction(ITU.WidgetActionType.None, WidgetEvent.Stopped, "", "");
            this.Action14 = new WidgetAction(ITU.WidgetActionType.None, WidgetEvent.Stopped, "", "");
            this.Action15 = new WidgetAction(ITU.WidgetActionType.None, WidgetEvent.Stopped, "", "");
        }

        [Browsable(false)]
        [DesignerSerializationVisibility(DesignerSerializationVisibility.Hidden)]
        new public virtual Color BackColor { get; set; }

        [Browsable(false)]
        [DesignerSerializationVisibility(DesignerSerializationVisibility.Hidden)]
        new public virtual Image BackgroundImage { get; set; }

        [Browsable(false)]
        [DesignerSerializationVisibility(DesignerSerializationVisibility.Hidden)]
        new public virtual ImageLayout BackgroundImageLayout { get; set; }

        [Browsable(false)]
        [DesignerSerializationVisibility(DesignerSerializationVisibility.Hidden)]
        new public BorderStyle BorderStyle { get; set; }

        [Browsable(false)]
        [DesignerSerializationVisibility(DesignerSerializationVisibility.Hidden)]
        new public virtual Cursor Cursor { get; set; }

        [Browsable(false)]
        [DesignerSerializationVisibility(DesignerSerializationVisibility.Hidden)]
        new public virtual Font Font { get; set; }

        [Browsable(false)]
        [DesignerSerializationVisibility(DesignerSerializationVisibility.Hidden)]
        new public virtual Color ForeColor { get; set; }

        [Browsable(false)]
        [DesignerSerializationVisibility(DesignerSerializationVisibility.Hidden)]
        new public virtual RightToLeft RightToLeft { get; set; }

        [Browsable(false)]
        [DesignerSerializationVisibility(DesignerSerializationVisibility.Hidden)]
        new public bool UseWaitCursor { get; set; }

        [Browsable(false)]
        [DesignerSerializationVisibility(DesignerSerializationVisibility.Hidden)]
        new public virtual bool AllowDrop { get; set; }

        [Browsable(false)]
        [DesignerSerializationVisibility(DesignerSerializationVisibility.Hidden)]
        new public virtual ContextMenuStrip ContextMenuStrip { get; set; }

        [Browsable(false)]
        [DesignerSerializationVisibility(DesignerSerializationVisibility.Hidden)]
        new public bool Enabled { get; set; }

        [Browsable(false)]
        [DesignerSerializationVisibility(DesignerSerializationVisibility.Hidden)]
        new public ImeMode ImeMode { get; set; }

        [Browsable(false)]
        [DesignerSerializationVisibility(DesignerSerializationVisibility.Hidden)]
        new public int TabIndex { get; set; }

        [Browsable(false)]
        [DesignerSerializationVisibility(DesignerSerializationVisibility.Hidden)]
        new public bool TabStop { get; set; }

        [Browsable(false)]
        [DesignerSerializationVisibility(DesignerSerializationVisibility.Hidden)]
        new public string AccessibleDescription { get; set; }

        [Browsable(false)]
        [DesignerSerializationVisibility(DesignerSerializationVisibility.Hidden)]
        new public string AccessibleName { get; set; }

        [Browsable(false)]
        [DesignerSerializationVisibility(DesignerSerializationVisibility.Hidden)]
        new public AccessibleRole AccessibleRole { get; set; }

        [Browsable(false)]
        [DesignerSerializationVisibility(DesignerSerializationVisibility.Hidden)]
        new public virtual AnchorStyles Anchor { get; set; }

        [Browsable(false)]
        [DesignerSerializationVisibility(DesignerSerializationVisibility.Hidden)]
        new public virtual bool AutoScroll { get; set; }

        [Browsable(false)]
        [DesignerSerializationVisibility(DesignerSerializationVisibility.Hidden)]
        new public Size AutoScrollMargin { get; set; }

        [Browsable(false)]
        [DesignerSerializationVisibility(DesignerSerializationVisibility.Hidden)]
        new public Size AutoScrollMinSize { get; set; }

        [Browsable(false)]
        [DesignerSerializationVisibility(DesignerSerializationVisibility.Hidden)]
        public override bool AutoSize { get; set; }

        [Browsable(false)]
        [DesignerSerializationVisibility(DesignerSerializationVisibility.Hidden)]
        new public virtual AutoSizeMode AutoSizeMode { get; set; }

        [Browsable(false)]
        [DesignerSerializationVisibility(DesignerSerializationVisibility.Hidden)]
        new public virtual DockStyle Dock { get; set; }

        [Browsable(false)]
        [DesignerSerializationVisibility(DesignerSerializationVisibility.Hidden)]
        new public Padding Margin { get; set; }

        [Browsable(false)]
        [DesignerSerializationVisibility(DesignerSerializationVisibility.Hidden)]
        new public virtual Size MaximumSize { get; set; }

        [Browsable(false)]
        [DesignerSerializationVisibility(DesignerSerializationVisibility.Hidden)]
        new public virtual Size MinimumSize { get; set; }

        [Browsable(false)]
        [DesignerSerializationVisibility(DesignerSerializationVisibility.Hidden)]
        new public Padding Padding { get; set; }

        [Browsable(false)]
        [DesignerSerializationVisibility(DesignerSerializationVisibility.Hidden)]
        new public bool CausesValidation { get; set; }

        [Browsable(false)]
        [DesignerSerializationVisibility(DesignerSerializationVisibility.Hidden)]
        new public ControlBindingsCollection DataBindings { get; set; }

        [Browsable(false)]
        [DesignerSerializationVisibility(DesignerSerializationVisibility.Hidden)]
        new public object Tag { get; set; }

        [LocalizedDescription("Delay", typeof(WidgetManual))]
        public int Delay { get; set; }
        [LocalizedDescription("TotalFrame", typeof(WidgetManual))]
        public int TotalFrame { get; set; }
        [LocalizedDescription("Repeat", typeof(WidgetManual))]
        public bool Repeat { get; set; }
        [LocalizedDescription("Playing", typeof(WidgetManual))]
        public bool Playing { get; set; }

        [LocalizedDescription("MoveAnimation", typeof(WidgetManual))]
        public bool MoveAnimation { get; set; }

        public enum MoveAnimationType
        {
            Linear,
            EaseIn,
            EaseOut
        };
        [LocalizedDescription("MoveType", typeof(WidgetManual))]
        public MoveAnimationType MoveType { get; set; }

        [LocalizedDescription("RotateAnimation", typeof(WidgetManual))]
        public bool RotateAnimation { get; set; }
        [LocalizedDescription("ScaleAnimation", typeof(WidgetManual))]
        public bool ScaleAnimation { get; set; }
        [LocalizedDescription("ColorAnimation", typeof(WidgetManual))]
        public bool ColorAnimation { get; set; }
        [LocalizedDescription("TransformAnimation", typeof(WidgetManual))]
        public bool TransformAnimation { get; set; }
        [LocalizedDescription("Cycling", typeof(WidgetManual))]
        public bool Cycling { get; set; }
        [LocalizedDescription("ScaleOnCenter", typeof(WidgetManual))]
        public bool ScaleOnCenter { get; set; }
        public bool MotionBlur { get; set; }
        [LocalizedDescription("AlwaysVisible", typeof(WidgetManual))]
        public bool AlwaysVisible { get; set; }

        public enum WidgetEvent
        {
            Stopped = 33
        };

        public class WidgetActionTypeConverter : TypeConverter
        {
            public override bool CanConvertFrom(ITypeDescriptorContext context, Type sourceType)
            {
                if (sourceType == typeof(string)) return true;
                return base.CanConvertFrom(context, sourceType);
            }

            // Return true if we need to convert into a string.
            public override bool CanConvertTo(ITypeDescriptorContext context, Type destinationType)
            {
                if (destinationType == typeof(String)) return true;
                return base.CanConvertTo(context, destinationType);
            }

            // Convert from a string.
            public override object ConvertFrom(ITypeDescriptorContext context, CultureInfo culture, object value)
            {
                if (value.GetType() == typeof(string))
                {
                    // Split the string separated by commas.
                    string txt = (string)(value);
                    string[] fields = txt.Split(new char[] { ',' });

                    try
                    {
                        ITU.WidgetActionType a = (ITU.WidgetActionType)Enum.Parse(typeof(ITU.WidgetActionType), fields[0]);
                        WidgetEvent e = WidgetEvent.Stopped;
                        try
                        {
                            e = (WidgetEvent)Enum.Parse(typeof(WidgetEvent), fields[1]);
                        }
                        catch
                        {
                            a = ITU.WidgetActionType.None;
                        }
                        return new WidgetAction(a, e, fields[2], fields[3]);
                    }
                    catch
                    {
                        throw new InvalidCastException(
                            "Cannot convert the string '" +
                            value.ToString() + "' into a Action");
                    }
                }
                else
                {
                    return base.ConvertFrom(context, culture, value);
                }
            }

            public override object ConvertTo(ITypeDescriptorContext context, System.Globalization.CultureInfo culture, object value, Type destinationType)
            {
                if (destinationType == typeof(string)) return value.ToString();
                return base.ConvertTo(context, culture, value, destinationType);
            }

            public override bool GetPropertiesSupported(ITypeDescriptorContext context)
            {
                return true;
            }

            public override PropertyDescriptorCollection GetProperties(ITypeDescriptorContext context, object value, Attribute[] attributes)
            {
                return TypeDescriptor.GetProperties(value);
            }
        }

        public class WidgetStringListConverter : TypeConverter
        {
            public override bool
            GetStandardValuesSupported(ITypeDescriptorContext context)
            {
                return true; // display drop
            }
            //public override bool
            //GetStandardValuesExclusive(ITypeDescriptorContext context)
            //{
            //    return true; // drop-down vs combo
            //}
            public override StandardValuesCollection
            GetStandardValues(ITypeDescriptorContext context)
            {
                List<string> names = new List<string>();
                WidgetAction a = (WidgetAction)context.Instance;

                foreach (HostSurface hs in HostSurface.hostSurfaces)
                {
                    Uitlity.GetWidgetNamesByActionType(hs.formDesignerHost.Container.Components, a.Action, names);
                }
                names.Sort();
                return new StandardValuesCollection(names.ToArray());
            }

            public override bool CanConvertFrom(System.ComponentModel.ITypeDescriptorContext context, System.Type sourceType)
            {
                if (sourceType == typeof(string))
                    return true;
                else
                    return base.CanConvertFrom(context, sourceType);
            }

            public override object ConvertFrom(System.ComponentModel.ITypeDescriptorContext context, System.Globalization.CultureInfo culture, object value)
            {
                if (value.GetType() == typeof(string))
                {
                    string s = value as string;
                    //NameCreationService.AddName(s);
                    return s;
                }
                else
                    return base.ConvertFrom(context, culture, value);
            }
        }

        [Serializable]
        [TypeConverter(typeof(WidgetActionTypeConverter))]
        public struct WidgetAction
        {
            public ITU.WidgetActionType Action { get; set; }
            public WidgetEvent Event { get; set; }

            private volatile string target;
            [TypeConverter(typeof(WidgetStringListConverter))]
            public String Target
            {
                get
                {
                    return target;
                }

                set
                {
                    if (Action == ITU.WidgetActionType.Function && ITU.enterKeyPressed && ITU.layerWidget != null && ITU.projectPath != null)
                    {
                        string funcName = value.Trim();

                        CodeGenerator.InvokeVisualStudio(funcName);

                        target = funcName;
                    }
                    else
                    {
                        target = value;
                    }
                }
            }

            public String Parameter { get; set; }

            public override string ToString()
            {
                return Action + "," + Event + "," + Target + "," + Parameter;
            }

            public WidgetAction(ITU.WidgetActionType action, WidgetEvent ev, String target, String param)
                : this()
            {
                Event = ev;
                Action = action;
                Target = target;
                Parameter = param;
            }
        }

        [LocalizedDescription("Action01to15",typeof(WidgetManual))]
        public WidgetAction Action01 { get; set; }
        [LocalizedDescription("Action01to15", typeof(WidgetManual))]
        public WidgetAction Action02 { get; set; }
        [LocalizedDescription("Action01to15", typeof(WidgetManual))]
        public WidgetAction Action03 { get; set; }
        [LocalizedDescription("Action01to15", typeof(WidgetManual))]
        public WidgetAction Action04 { get; set; }
        [LocalizedDescription("Action01to15", typeof(WidgetManual))]
        public WidgetAction Action05 { get; set; }
        [LocalizedDescription("Action01to15", typeof(WidgetManual))]
        public WidgetAction Action06 { get; set; }
        [LocalizedDescription("Action01to15", typeof(WidgetManual))]
        public WidgetAction Action07 { get; set; }
        [LocalizedDescription("Action01to15", typeof(WidgetManual))]
        public WidgetAction Action08 { get; set; }
        [LocalizedDescription("Action01to15", typeof(WidgetManual))]
        public WidgetAction Action09 { get; set; }
        [LocalizedDescription("Action01to15", typeof(WidgetManual))]
        public WidgetAction Action10 { get; set; }
        [LocalizedDescription("Action01to15", typeof(WidgetManual))]
        public WidgetAction Action11 { get; set; }
        [LocalizedDescription("Action01to15", typeof(WidgetManual))]
        public WidgetAction Action12 { get; set; }
        [LocalizedDescription("Action01to15", typeof(WidgetManual))]
        public WidgetAction Action13 { get; set; }
        [LocalizedDescription("Action01to15", typeof(WidgetManual))]
        public WidgetAction Action14 { get; set; }
        [LocalizedDescription("Action01to15", typeof(WidgetManual))]
        public WidgetAction Action15 { get; set; }

        private bool hided = false;
        [LocalizedDescription("Hided", typeof(WidgetManual))]
        public bool Hided
        {
            get
            {
                return hided;
            }

            set
            {
                if (value)
                    Hide();
                else
                    Show();

                hided = value;
            }
        }

        private bool hide_ds = false;
        public bool HideDS
        {
            get
            {
                return hide_ds;
            }
            set
            {
                hide_ds = value;
            }
        }

        public ITUWidget CreateITUWidget()
        {
            ITUSimpleAnimation animation = new ITUSimpleAnimation();

            animation.name = this.Name;
            animation.flags |= this.AlwaysVisible ? ITU.ITU_ALWAYS_VISIBLE : 0;

            PropertyDescriptorCollection properties = TypeDescriptor.GetProperties(this);
            animation.visible = (bool)properties["Visible"].GetValue(this);

            animation.active = false;
            animation.dirty = false;
            animation.alpha = 255;
            animation.rect.x = this.Location.X;
            animation.rect.y = this.Location.Y;
            animation.rect.width = this.Size.Width;
            animation.rect.height = this.Size.Height;
            animation.color.alpha = 255;
            animation.color.red = 0;
            animation.color.green = 0;
            animation.color.blue = 0;
            animation.bound.x = 0;
            animation.bound.y = 0;
            animation.bound.width = 0;
            animation.bound.height = 0;

            animation.delay = this.Delay;
            animation.totalframe = this.TotalFrame;
            animation.repeat = this.Repeat ? 1 : 0;
            animation.playing = this.Playing ? 1 : 0;

            animation.animationFlags = 0;

            if (this.MoveAnimation)
            {
                if (this.MoveType == MoveAnimationType.Linear)
                    animation.animationFlags |= ITUAnimation.ITU_ANIM_MOVE;
                else if (this.MoveType == MoveAnimationType.EaseIn)
                    animation.animationFlags |= ITUAnimation.ITU_ANIM_EASE_IN;
                else if (this.MoveType == MoveAnimationType.EaseOut)
                    animation.animationFlags |= ITUAnimation.ITU_ANIM_EASE_OUT;
            }
            
            if (this.RotateAnimation)
                animation.animationFlags |= ITUAnimation.ITU_ANIM_ROTATE;

            if (this.ScaleAnimation)
                animation.animationFlags |= ITUAnimation.ITU_ANIM_SCALE;

            if (this.ColorAnimation)
                animation.animationFlags |= ITUAnimation.ITU_ANIM_COLOR;

            if (this.TransformAnimation)
                animation.animationFlags |= ITUAnimation.ITU_ANIM_TRANSFORM;

            if (this.Cycling)
                animation.animationFlags |= ITUAnimation.ITU_ANIM_CYCLE;

            if (this.ScaleOnCenter)
                animation.animationFlags |= ITUAnimation.ITU_ANIM_SCALE_CENTER;

            if (this.MotionBlur)
                animation.animationFlags |= ITUAnimation.ITU_ANIM_MOTION_BLUR;

            animation.actions[0].action = (ITUActionType)this.Action01.Action;
            animation.actions[0].ev = (ITUEvent)this.Action01.Event;
            animation.actions[0].target = this.Action01.Target;
            animation.actions[0].param = this.Action01.Parameter;
            animation.actions[1].action = (ITUActionType)this.Action02.Action;
            animation.actions[1].ev = (ITUEvent)this.Action02.Event;
            animation.actions[1].target = this.Action02.Target;
            animation.actions[1].param = this.Action02.Parameter;
            animation.actions[2].action = (ITUActionType)this.Action03.Action;
            animation.actions[2].ev = (ITUEvent)this.Action03.Event;
            animation.actions[2].target = this.Action03.Target;
            animation.actions[2].param = this.Action03.Parameter;
            animation.actions[3].action = (ITUActionType)this.Action04.Action;
            animation.actions[3].ev = (ITUEvent)this.Action04.Event;
            animation.actions[3].target = this.Action04.Target;
            animation.actions[3].param = this.Action04.Parameter;
            animation.actions[4].action = (ITUActionType)this.Action05.Action;
            animation.actions[4].ev = (ITUEvent)this.Action05.Event;
            animation.actions[4].target = this.Action05.Target;
            animation.actions[4].param = this.Action05.Parameter;
            animation.actions[5].action = (ITUActionType)this.Action06.Action;
            animation.actions[5].ev = (ITUEvent)this.Action06.Event;
            animation.actions[5].target = this.Action06.Target;
            animation.actions[5].param = this.Action06.Parameter;
            animation.actions[6].action = (ITUActionType)this.Action07.Action;
            animation.actions[6].ev = (ITUEvent)this.Action07.Event;
            animation.actions[6].target = this.Action07.Target;
            animation.actions[6].param = this.Action07.Parameter;
            animation.actions[7].action = (ITUActionType)this.Action08.Action;
            animation.actions[7].ev = (ITUEvent)this.Action08.Event;
            animation.actions[7].target = this.Action08.Target;
            animation.actions[7].param = this.Action08.Parameter;
            animation.actions[8].action = (ITUActionType)this.Action09.Action;
            animation.actions[8].ev = (ITUEvent)this.Action09.Event;
            animation.actions[8].target = this.Action09.Target;
            animation.actions[8].param = this.Action09.Parameter;
            animation.actions[9].action = (ITUActionType)this.Action10.Action;
            animation.actions[9].ev = (ITUEvent)this.Action10.Event;
            animation.actions[9].target = this.Action10.Target;
            animation.actions[9].param = this.Action10.Parameter;
            animation.actions[10].action = (ITUActionType)this.Action11.Action;
            animation.actions[10].ev = (ITUEvent)this.Action11.Event;
            animation.actions[10].target = this.Action11.Target;
            animation.actions[10].param = this.Action11.Parameter;
            animation.actions[11].action = (ITUActionType)this.Action12.Action;
            animation.actions[11].ev = (ITUEvent)this.Action12.Event;
            animation.actions[11].target = this.Action12.Target;
            animation.actions[11].param = this.Action12.Parameter;
            animation.actions[12].action = (ITUActionType)this.Action13.Action;
            animation.actions[12].ev = (ITUEvent)this.Action13.Event;
            animation.actions[12].target = this.Action13.Target;
            animation.actions[12].param = this.Action13.Parameter;
            animation.actions[13].action = (ITUActionType)this.Action14.Action;
            animation.actions[13].ev = (ITUEvent)this.Action14.Event;
            animation.actions[13].target = this.Action14.Target;
            animation.actions[13].param = this.Action14.Parameter;
            animation.actions[14].action = (ITUActionType)this.Action15.Action;
            animation.actions[14].ev = (ITUEvent)this.Action15.Event;
            animation.actions[14].target = this.Action15.Target;
            animation.actions[14].param = this.Action15.Parameter;

            return animation;
        }

        public void SaveImages(String path)
        {
        }

        public void WriteFunctions(HashSet<string> functions)
        {
            if (this.Action01.Action == ITU.WidgetActionType.Function)
                functions.Add(this.Action01.Target);
            if (this.Action02.Action == ITU.WidgetActionType.Function)
                functions.Add(this.Action02.Target);
            if (this.Action03.Action == ITU.WidgetActionType.Function)
                functions.Add(this.Action03.Target);
            if (this.Action04.Action == ITU.WidgetActionType.Function)
                functions.Add(this.Action04.Target);
            if (this.Action05.Action == ITU.WidgetActionType.Function)
                functions.Add(this.Action05.Target);
            if (this.Action06.Action == ITU.WidgetActionType.Function)
                functions.Add(this.Action06.Target);
            if (this.Action07.Action == ITU.WidgetActionType.Function)
                functions.Add(this.Action07.Target);
            if (this.Action08.Action == ITU.WidgetActionType.Function)
                functions.Add(this.Action08.Target);
            if (this.Action09.Action == ITU.WidgetActionType.Function)
                functions.Add(this.Action09.Target);
            if (this.Action10.Action == ITU.WidgetActionType.Function)
                functions.Add(this.Action10.Target);
            if (this.Action11.Action == ITU.WidgetActionType.Function)
                functions.Add(this.Action11.Target);
            if (this.Action12.Action == ITU.WidgetActionType.Function)
                functions.Add(this.Action12.Target);
            if (this.Action13.Action == ITU.WidgetActionType.Function)
                functions.Add(this.Action13.Target);
            if (this.Action14.Action == ITU.WidgetActionType.Function)
                functions.Add(this.Action14.Target);
            if (this.Action15.Action == ITU.WidgetActionType.Function)
                functions.Add(this.Action15.Target);
        }

        public bool HasFunctionName(string funcName)
        {
            if (this.Action01.Action == ITU.WidgetActionType.Function && this.Action01.Target == funcName)
                return true;
            if (this.Action02.Action == ITU.WidgetActionType.Function && this.Action02.Target == funcName)
                return true;
            if (this.Action03.Action == ITU.WidgetActionType.Function && this.Action03.Target == funcName)
                return true;
            if (this.Action04.Action == ITU.WidgetActionType.Function && this.Action04.Target == funcName)
                return true;
            if (this.Action05.Action == ITU.WidgetActionType.Function && this.Action05.Target == funcName)
                return true;
            if (this.Action06.Action == ITU.WidgetActionType.Function && this.Action06.Target == funcName)
                return true;
            if (this.Action07.Action == ITU.WidgetActionType.Function && this.Action07.Target == funcName)
                return true;
            if (this.Action08.Action == ITU.WidgetActionType.Function && this.Action08.Target == funcName)
                return true;
            if (this.Action09.Action == ITU.WidgetActionType.Function && this.Action09.Target == funcName)
                return true;
            if (this.Action10.Action == ITU.WidgetActionType.Function && this.Action10.Target == funcName)
                return true;
            if (this.Action11.Action == ITU.WidgetActionType.Function && this.Action11.Target == funcName)
                return true;
            if (this.Action12.Action == ITU.WidgetActionType.Function && this.Action12.Target == funcName)
                return true;
            if (this.Action13.Action == ITU.WidgetActionType.Function && this.Action13.Target == funcName)
                return true;
            if (this.Action14.Action == ITU.WidgetActionType.Function && this.Action14.Target == funcName)
                return true;
            if (this.Action15.Action == ITU.WidgetActionType.Function && this.Action15.Target == funcName)
                return true;
            return false;
        }
    }
}
